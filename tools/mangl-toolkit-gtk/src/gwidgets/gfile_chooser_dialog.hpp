/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gwindow.hpp"

#include <apeal/signal.hpp>

namespace gw {

struct FileChooserFile {
    String path;
    String baseName;
    // String absolutePath;
};

struct FileChooserResult {
    using FilesContainer = std::vector<FileChooserFile>;
    String currentDirectory;
    FilesContainer files;
};

class FileChooserDialog {
    using Self = FileChooserDialog;
    using SelfP = Self*;

public:
    apl::Signal<FileChooserResult> responseSignal;

    FileChooserDialog() = default;

    auto setSelectMultiple(bool flag = true) {
        selectMultiple_ = flag;
        updateSelectMultiple();
        return this;
    }

    auto setCurrentDirectory(const char* dir) {
        currentDirectory_ = dir;
        return this;
    }

    auto setCurrentDirectory(StrView dir) {
        currentDirectory_ = dir;
        return this;
    }

    auto addFilterSuffix(const char* suffix) {
        // TODO:
        // if (!filter_) {
        //     filter_ = gtk_file_filter_new();
        //
        //     if (dialog_) {
        //         gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog_), filter_);
        //         gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog_), filter_);
        //     }
        // }
        // gtk_file_filter_add_suffix(filter_, suffix);
        return this;
    }

    template <class S>
    auto addFilterSuffixes(std::initializer_list<S> suffixes) {
        for (auto& suffix: suffixes) {
            addFilterSuffix(suffix);
        }
        return this;
    }

    void show(Window* parent) {
        // GtkWindow* parentWindow{};
        dialog_ = gtk_file_dialog_new();

        // dialog_ = gtk_file_chooser_dialog_new(
        //     "Open File", parent->gwindow(), action_, "_Cancel", GTK_RESPONSE_CANCEL, "_Open",
        //     GTK_RESPONSE_ACCEPT, NULL
        // );

        // TODO:
        // if (filter_) {
        //     gtk_file_dialog_set_filters(dialog_, filter_);
        //     //gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog_), filter_);
        // }

        updateSelectMultiple();
        updateCurrentDirectory();

        gtk_window_present(GTK_WINDOW(dialog_));

        g_signal_connect(dialog_, "response", G_CALLBACK(response_cb), this);
    }

private:
    bool selectMultiple_{};
    //char const* currentDirectory_{};
    String currentDirectory_;

    void updateSelectMultiple() {
        if (!dialog_)
            return;
        // TODO:
        //gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog_), selectMultiple_);
    }

    void updateCurrentDirectory() {
        // TODO:
        // if (!dialog_)
        //     return;
        // if (currentDirectory_.empty())
        //     return;
        //
        // GError* error{};
        // auto gfile = g_file_new_for_path(currentDirectory_.c_str());
        // auto res = gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog_), gfile, &error);
        // g_object_unref(gfile);
    }

private:
    static void response_cb(GtkDialog* dialog, int response, gpointer userData) {
        static_cast<SelfP>(userData)->reponse_cb(dialog, response);
    }

    void reponse_cb(GtkDialog* dialog, int response) {
        // TODO:
        /*
        if (GTK_RESPONSE_ACCEPT == response) {
            auto chooser = GTK_FILE_CHOOSER(dialog);

            auto selectedFiles = gtk_file_chooser_get_files(chooser);

            auto n = g_list_model_get_n_items(selectedFiles);

            FileChooserResult result;

            for (guint i = 0; i < n; i++) {
                auto gfile = (GFile*)g_list_model_get_item(selectedFiles, i);

                FileChooserFile chFile;

                auto path = g_file_get_path(gfile);
                chFile.path = path;
                g_free(path);

                path = g_file_get_basename(gfile);
                chFile.baseName = path;
                g_free(path);


                g_object_unref(gfile);

                result.files.emplace_back(chFile);
            }

            // g_autoptr(GFile) gfile = gtk_file_chooser_get_file(chooser);

            g_object_unref(selectedFiles);

            result.currentDirectory.clear();

            if (GFile* gCurrentPath = gtk_file_chooser_get_current_folder(chooser); gCurrentPath) {

                if (auto path = g_file_get_path(gCurrentPath); path) {
                    result.currentDirectory = path;
                    g_free(path);
                }
                g_object_unref(gCurrentPath);
            }

            responseSignal(result);
        } // If accept files


        gtk_window_destroy(GTK_WINDOW(dialog));
        // g_object_unref(dialog_);
        dialog_ = nullptr;
        */
    }

    GtkFileChooserAction action_ = GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkFileDialog* dialog_{};
    //GtkWidget* dialog_{};
    GtkFileChooser* chooser_{};
    //GtkFileFilter* filter_{};
};


// namespace impl {
// static void response_cb(GtkDialog* dialog, int response, gpointer userData) {
//     static_cast<FileChooserDialog*>(userData)->reponse_cb(dialog, response);
// }
//
// } // namespace impl

} // namespace gw
