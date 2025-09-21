/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
/*

	Builds AE project files for OpenSesame plugin: http://aescripts.com/pt_opensesame/

*/

#include <iostream>
#include <string>

#include <apeal/file.hpp>
#include <apeal/parse.hpp>

#include <map>
#include <set>
#include <sstream>

typedef std::string string_type ;

string_type audio_info_filename_, video_info_filename_, tsv_filename_ ;


string_type root_dir = "/c/data/capture" ;
string_type composition_name = "SHW" ;
int composition_width = 1136, composition_height = 640 ;
double composition_duration = 1 ;
int composition_fps = 30 ;
int composition_id = 0 ;


void process()
{
	using namespace apl ;
	using namespace std ;

	vector<vector<string_type> > audio_info_data, video_info_data ;

	ACsvParser csv_parser ;
	AFileSystem fs ;

	csv_parser.load ( audio_info_filename_, audio_info_data ) ;
	csv_parser.load ( video_info_filename_, video_info_data ) ;


	int global_id = 1 ;

	//struct AudioLayer {
	//	double timestamp ;
	//	string_type file ;
	//	int footage_id ;
	//} ;

	struct Footage {
		string_type file ;
		int footage_id ;
		string_type type ;
		int width, height ;

		Footage()
		{
			width = height = 0 ;
		}
	} ;

	struct Layer {
		double begin_timestamp, end_timestamp ;
		string_type file ;
		int footage_id ;
		int video ;
		int audio ;
	} ;

	vector<Layer> layers ;
	//vector<VideoLayer> video_records ;
	map<string_type,Footage> footages ;


	//
	// Video
	//
	for ( auto r_begin = video_info_data.begin(), r = r_begin, rn = r_begin, r_end = video_info_data.end() ; r != r_end ; ++r ) {
		if ( r == r_begin ) {
			parse_int(r->at(0),composition_width) ;
			parse_int(r->at(1),composition_height) ;
			parse_int(r->at(2),composition_fps) ;
			++rn ;
			++rn ;
			continue ;
		}

		Layer l ;
		parse_float(r->at(0),l.begin_timestamp) ;
		l.file = r->at(1) ;
		l.video = 1 ;
		l.audio = 0 ;

		if ( rn != r_end ) {
			parse_float(rn->at(0),l.end_timestamp) ;
		} else {
			l.end_timestamp = l.begin_timestamp + 2 / float(composition_fps) ;
		}

		if ( l.end_timestamp > composition_duration )
			composition_duration = l.end_timestamp ;

		auto footage_it = footages.find(l.file) ;
		if ( footage_it == footages.end() ) {
			Footage f ;
			f.file = l.file ;
			l.footage_id = f.footage_id = global_id++ ;
			f.type = "Still" ;
			f.width = composition_width ;
			f.height = composition_height ;
			footages.insert(pair<string_type,Footage>(f.file,f)) ;
		} else {
			l.footage_id = footage_it->second.footage_id ;
		}


		layers.push_back(l) ;

		if ( rn != r_end )
			++rn ;
	}


	//
	// Audio
	//
	for ( auto r = audio_info_data.begin() ; r != audio_info_data.end() ; ++r ) {
		Layer l ;
		parse_float(r->at(0),l.begin_timestamp) ;
		l.end_timestamp = l.begin_timestamp + 3 ;
		l.video = 0 ;
		l.audio = 1 ;

		fs.extract_file_name ( r->at(1), l.file ) ;

		auto footage_it = footages.find(l.file) ;
		if ( footage_it == footages.end() ) {
			Footage f ;
			f.file = l.file ;
			l.footage_id = f.footage_id = global_id++ ;
			f.type = "Audio" ;

			footages.insert(pair<string_type,Footage>(f.file,f)) ;
		} else {
			l.footage_id = footage_it->second.footage_id ;
		}

		layers.push_back(l) ;
	}



	stringstream tsv ;

	tsv.precision(16) ;
	tsv.unsetf ( ios::floatfield ) ;

	int footage_count = footages.size() ;
	int compositions_count = 0 ;
	int solids_count = 0 ;
	int layers_count = 0 ;

	char t = '\t' ;


	tsv << "pt_OpenSesame" << t << "Version:1.1" << '\t' << "Export Folder:" << root_dir << endl ;

	tsv << "Project" << t << "BitsPerChannel:8" << t << "Blending:0" << t << "StartFrame:TIMECODE" << t << "FootageStart:SOURCE" << t << "Display:TIMECODE" << t << "DropFrame:0" << endl ;

	tsv << "Footage Section" << '\t' << footage_count << endl ;

	for ( auto f_it = footages.begin(), f_end = footages.end() ; f_it != f_end ; ++f_it ) {
		const auto &f = f_it->second ;
		tsv << "Footage" << t << "Name:" << f.file << t << "ID:" << f.footage_id << t << "FolderID:0" << t << "Type:" << f.type << t << "Width:" << f.width << t << "Height:" << f.height << t << "FieldSep:OFF" << t << "PullDown:OFF" << t << "Aspect:1" << t << "Path:" << root_dir << "/" << f.file << t << "Relative Path:" << f.file << t <<  "Label:7" << endl ;
	}


	compositions_count = 1 ;
	composition_id = global_id++ ;
	tsv << "Comp Section" << t << compositions_count << endl ;
	tsv << "Comp" << t << "Name:" << composition_name << t << "ID:" << composition_id << t << "FolderID:0" << t << "Width:" << composition_width << t << "Height:" << composition_height << t << "Resolution:1\tMB:0\tFB:0\tAngle:180\tPhase:-90\tShy:0\tPreserveRate:0\tPreserveRes:0\tAspect:1" << t << "Duration:" << composition_duration << t << "FPS:" << composition_fps << t << "BgColor:0;0;0" << t << "DropFrame:0" << t << "Label:15" << endl ;

	tsv << "Solid Section" << t << solids_count << endl ;

	layers_count = layers.size() ; 
	tsv << "Layer Section" << t << layers_count << endl ;

	for ( auto l_it = layers.begin(), l_end = layers.end() ; l_it != l_end ; ++l_it ) {
		const auto &l = *l_it ;

		tsv << "Layer" << t << "Name:" << l.file << t << "ID:" << l.footage_id << t << "CompID:" << composition_id << t << "Type:Footage" << t << "Video:" << l.video << t << "Audio:" << l.audio << t << "AutoOrient:NONE" << t << "Blend:NORMAL" << t << "UsedIn:1" << t << "startTime:" << l.begin_timestamp << t << "inPoint:" << l.begin_timestamp << t << "outPoint:" << l.end_timestamp << t << "Label:7" << endl ;
		//global_id++ ;
	}
	

	//
	// Rendering
	//
	tsv << "Render" << t << "CompID:" << composition_id << t << "Start:0" << t << "Duration:" << composition_duration << t << "Skip:0" << t << "RenderSettings:Default" << t << "OutputModules:1" << endl ;
	tsv << "OutputModule" << t << "Name:Lossless" << t << "Path:" << root_dir << "/" << composition_name << ".mov" << endl ;


	AFile<> file ;
	file.save(tsv_filename_,tsv.str()) ;

}


int main ( int argc, char *argv[] )
{
	using namespace std ;
	using namespace apl ;

	int ret_code = 1 ;

	try {
		
		for ( int i = 1 ; i < argc ; ++i ) {
			if ( argv[i][0] == '-' ) {  // option
				string_type option = argv[i] ;
				string_type name, value ;

				str_split ( option, '=', name, value ) ;

				if ( "--help" == name ) {
					cout << "build_capture_tsv: [<option>]" << endl ;
					cout << "options:" << endl ;
					cout << "  --video-info=<file>" << endl ;
					cout << "  --autio-info=<file>" << endl ;
					cout << "  --output=<file>" << endl ;
					return 1 ;
				} 

				if ( "--video-info" == name )			video_info_filename_ = value ; else
				if ( "--audio-info" == name )		audio_info_filename_ = value ; else
				if ( "--output" == name )			tsv_filename_ = value ; else
				throw runtime_error("Invalid option: " + option) ;
			}
		}


		process() ;
	}
	catch(exception &e) {
		cerr << e.what() << endl ;
	}
	catch(...) {
		cerr << "exception!" << endl ;
	}

	return ret_code ;
}




