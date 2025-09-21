// @ts-check


// @ts-ignore: node imports
import fs from 'fs';

let content_ = '';

let upscales_ = [];
let fonts_ = [];
let textures_ = [];
let fontDeps_ = [];
let textureDeps_ = [];
let defines_ = [];


/** @param {string} s */
let outn = (s) => { content_ += s + '\n'; }

/** @param {string} s */
let outnn = (s) => { content_ += s + '\n\n'; }


/**
 * @param {number=} count
**/
function nl(count) {
    let c = count ?? 1;
    for (let i = 0; i < c; ++i)
        content_ += '\n';
}

function generateHeader() {
    let genDate = new Date();
    let dateString = genDate.toISOString().split('T')[0];
    content_ = "";
    outn(`# *** Automatically Generated *** DO NOT EDIT ***`);
    outn(`# * Generated Date: ${dateString}`);
    nl();
}

/** @param {string} s */
function toNinjaVar(s) {
    return s.replace('(', '{').replace(')', '}');
}

/** @param {string} s */
function toMakeVar(s) {
    return s.replace('{', '(').replace('}', ')');
}


/**
 * 
 * @param {import ("./gengraphics.d.ts").ProjectConfig} config
 * @param {string} descfile
 */
async function readYaml(config, descfile){

    upscales_ = [];
    fonts_ = [];
    textures_ = [];

    const YAML = await import(`${config.apeal_scripts_dir}/js/yaml.mjs`);

    const doc = YAML.parse(fs.readFileSync(descfile, 'utf8'));

    //console.log(doc);

    let graphics = doc.graphics;
    
    upscales_ = [];
    if (graphics.upscaleSteps) {
        for (let upscale of graphics.upscaleSteps) {
            upscales_.push(upscale);
        } 
    } 
    if (upscales_.length == 0){
        upscales_.push(1)
    }

    fonts_ = [];
    for (let font of graphics.fonts) {
        fonts_.push(font);
    }

    textures_ = [];
    for (let texture of graphics.textures) {
        textures_.push(texture);
    }

    defines_ = [];
    if (graphics.defines) {
        for (let define of graphics.defines) {
            defines_.push(define)
        }
    }


    fontDeps_ = [];
    for (let font of fonts_) {
        fontDeps_.push(font.name);

        if (font.sizes) {
            for (let size of font.sizes) {
                fontDeps_.push(`${font.name}${size.suffix}`);
            }
        }
    }

    textureDeps_ = [];
    for (const tex of textures_) {
        textureDeps_.push(tex.name);
    }




    return doc.graphics;
}


/**
 * 
 * @param {import ("./gengraphics.d.ts").GenerateGraphicsParams} params
 */
export async function generateMake(params){

    let graphics = await readYaml(params.config, params.desc);

    generateHeader();

    outnn(`include ${params.include}`);

    outn(`TEXTURES_DIR := ${toMakeVar(graphics.texturesDir)}`);
    outn(`RENDER_DIR := ${toMakeVar(graphics.renderDir)}`);
    outn(`RENDER_FONT_DIR := ${toMakeVar(graphics.renderFontsDir)}`);
    outn(`FONTS_DIR := ${toMakeVar(graphics.fontsDir)}`);
    outn(`SRC_GEN_DIR := ${toMakeVar(graphics.srcGenDir)}`);
    
    nl();

    outnn(`DEPS_DIR := .deps`);

    outn(`texmake := ${graphics.texmakeBin ?? "mangl-make-tex"}`);
    outn(`fontmake := mangl-font-render`);
    nl();

    outn(`FONTMAKE_PARAMS :=`)
    outn(`TEXMAKE_PARAMS := ${graphics.texmakeParams ?? ""}`)
    nl()



    outnn(`fonts := ${fontDeps_.join(' ')}`);
    outnn(`textures := ${textureDeps_.join(' ')}`);
    
    outnn('all: $(fonts) $(textures)');
    outnn('.PHONY: all $(fonts) $(textures)');


    for (let tex of textureDeps_) {
        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;
            outn(`-include $(DEPS_DIR)/${tex}_desc.yaml${upsfx}.d`);
        }
    }
    nl();


    outnn("$(DEPS_DIR): ; @mkdir -p $@");

    // Generates
    // Makefile: <graphics>.yaml
    //    <makefile-build-script>
    if (graphics.generator) {
        outn(`${params.target}: ${params.desc}`);
        outnn(`\t${graphics.generator}`);
    }

    

    outn("#");
    outn('# Generating Fonts')
    outn("#");
    nl();

    for (let font of fonts_) {

        // Font dependencies
        content_ += `${font.name}:`;
        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;
            let pathBase = `$(RENDER_FONT_DIR)/${font.name}${upsfx}`;

            content_ += ` ${pathBase}.yaml ${pathBase}.png`;


            if (font.sizes) {
                for (let size of font.sizes) {
                    pathBase = `$(RENDER_FONT_DIR)/${font.name}${size.suffix}${upsfx}`;
                    content_ += ` ${pathBase}.yaml ${pathBase}.png`;
                }
            }
        }

        nl();
        nl();

        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;
            let pathBase = `$(RENDER_FONT_DIR)/${font.name}${upsfx}`;

            outn(`${pathBase}.yaml ${pathBase}.png: ${params.target}`);
            outn(`\t@echo ${font.name}${upsfx}...`);
            outnn(`\t@$(fontmake) \$(FONTMAKE_PARAMS) --input=\$(FONTS_DIR)/${font.file} --height=${font.height * upscale} -o ${pathBase}.yaml ${pathBase}.png`);

            if (font.sizes) {
                for (let size of font.sizes) {
                    pathBase = `$(RENDER_FONT_DIR)/${font.name}${size.suffix}${upsfx}`;

                    outn(`${pathBase}.yaml ${pathBase}.png:`);
                    outn(`\t@echo ${font.name}${size.suffix}${upsfx}...`);
                    outnn(`\t@$(fontmake) \$(FONTMAKE_PARAMS) --input=\$(FONTS_DIR)/${font.file} --height=${size.height * upscale} -o ${pathBase}.yaml ${pathBase}.png`);
                }
            }


        }
    }


    nl();

    outn("#");
    outn('# Generating Textures')
    outn("#");
    nl();

    for (let tex of textures_) {
        let textureDeps = [];

        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;

            let basePath = `$(TEXTURES_DIR)/${tex.name}${upsfx}`;
            textureDeps.push(`${basePath}.yaml`);
            textureDeps.push(`${basePath}.png`);

            if (upscale === 1 && !params.nohpp) {
                textureDeps.push(`$(SRC_GEN_DIR)/${tex.name}.hpp`);
            }
        }

        outnn(`${tex.name}: ${textureDeps.join(' ')} ${fontDeps_.join(' ')} ${params.target}`);


        // Generating c++ header file
        if (!params.nohpp) {
            let cmdp = [];

            cmdp.push(`--input=${tex.name}_desc.yaml`);

            if (defines_.length > 0) {
                let sdefines = '';
                for (let def of defines_) {
                    if (sdefines.length > 0) sdefines += ';';
                    sdefines += `${def.name}=${toMakeVar(def.value)}`;
                }
                cmdp.push(`--define=${sdefines}`);
            }


            let targets = [];
            targets.push(`$(SRC_GEN_DIR)/${tex.name}.hpp`);

            outn(`${targets.join(' ')}: ${tex.name}_desc.yaml`);
            outn(`\t@echo ${tex.name}.hpp...`);
            outn(`\t@$(texmake) \$(TEXMAKE_PARAMS) ${cmdp.join(' ')} -o ${targets.join(' ')}`);
            nl();
        }


        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;

            let basePath = `$(TEXTURES_DIR)/${tex.name}${upsfx}`;

            let cmdp = [];

            cmdp.push(`--input=${tex.name}_desc.yaml`);
            cmdp.push(`--deps=$(DEPS_DIR)/${tex.name}_desc.yaml${upsfx}.d`);


            if (upscale > 1) {
                cmdp.push(`--scale=${upscale}`);
                cmdp.push(`--suffix=-${upscale}x`);
                cmdp.push(`--density=${upscale}`);
            }

            if (defines_.length > 0) {
                let sdefines = '';
                for (let def of defines_) {
                    if (sdefines.length > 0)
                        sdefines += ';';
                    sdefines += `${def.name}=${toMakeVar(def.value)}`;
                }
                cmdp.push(`--define=${sdefines}`);
            }

            let targets = [];
            targets.push(`${basePath}.yaml`);
            targets.push(`${basePath}.png`);


            outn(`${targets.join(' ')}: ${tex.name}_desc.yaml`);
            outn(`${targets.join(' ')}:`);
            outn(`\t@echo ${tex.name}${upsfx}...`);
            outn(`\t@$(texmake) \$(TEXMAKE_PARAMS) ${cmdp.join(' ')} -o ${targets.join(' ')}`);
            nl();
        }

        nl();
    }

    nl();

    fs.writeFileSync(params.target, content_);
}



/**
 * 
 * @param {import ("./gengraphics.d.ts").GenerateGraphicsParams} params
 */
export async function generateNinja(params){

    let graphics = await readYaml(params.config, params.desc);


    generateHeader();


    outnn(`include ${params.include}`);

    outn(`TEXTURES_DIR = ${toNinjaVar(graphics.texturesDir)}`);
    outn(`RENDER_DIR = ${toNinjaVar(graphics.renderDir)}`);
    outn(`RENDER_FONT_DIR = ${toNinjaVar(graphics.renderFontsDir)}`);
    outn(`FONTS_DIR = ${toNinjaVar(graphics.fontsDir)}`);
    outn(`SRC_GEN_DIR = ${toNinjaVar(graphics.srcGenDir)}`);
    
    nl();

    outnn(`DEPS_DIR = .deps`);


    outn('texmake = mangl-make-tex');
    outn('fontmake = mangl-font-render');
    nl();

    outn(`FONTMAKE_PARAMS =`)
    outn(`TEXMAKE_PARAMS = ${graphics.texmakeParams ?? ""}`)
    nl()


    let defineParam = '';

    if (defines_.length > 0) {
        let sdefines = '';
        for (let def of defines_) {
            if (sdefines.length > 0)
                sdefines += ';';
            sdefines += `${def.name}=${toNinjaVar(def.value)}`;
        }

        defineParam = `--define=${sdefines}`;
    }



    nl();
    outn("#");
    outn("# Rules");
    outn("#");
    nl();

    if (graphics.generator) {
        outn('rule ninja')
        outn(`    command = ${graphics.generator}`);
        outn('    description = Making build.ninja')
        nl();
    }

    outn('rule fontmake')
    outn(`    command = $fontmake \${FONTMAKE_PARAMS} --input=\${in} --height=$height --output=$out`);
    outn('    description = Making font $in')
    nl();

    if (!params.nohpp) {
        outn('rule texmake_h')
        outn(`    command = $texmake \${TEXMAKE_PARAMS} --input=\${in} ${defineParam} --output=$out`)
        outn('    description = Making texture $out')
        nl();
    }

    for(const upscale of upscales_){
        let upsfx = upscale === 1 ? "" : `-${upscale}x`;

        let cmdp = [];

        if (upscale > 1) {
            cmdp.push(`--scale=${upscale}`);
            cmdp.push(`--suffix=-${upscale}x`);
            cmdp.push(`--density=${upscale}`);
        }

        cmdp.push("--input=$in");
        cmdp.push(`--deps=.deps/\${in}${upsfx}.d`);

        if (defineParam.length > 0)
            cmdp.push(defineParam);

        cmdp.push("--output=$out");

        outn(`rule texmake${upsfx}`);
        outn(`    depfile = .deps/\${in}${upsfx}.d`);
        outn(`    command = $texmake ${cmdp.join(' ')}`);
        outn(`    description = Making texture $out`);

        nl();
    }


    outn("#");
    outn("# Generic builds");
    outn("#");
    nl();
    if (graphics.generator) {
        outn(`build build.ninja: ninja | ${params.desc}`);
        nl();
    }


    outn("#");
    outn("# Font dependencies");
    outn("#");
    nl();

    let buildDeps = [];

    for (let font of fonts_) {
        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;
            let pathBase = `\${RENDER_FONT_DIR}/${font.name}${upsfx}`;


            buildDeps.push(`${pathBase}.yaml`);
            buildDeps.push(`${pathBase}.png`);

            outn(`build ${pathBase}.yaml ${pathBase}.png: fontmake \${FONTS_DIR}/${font.file}`);
            outn(`    height = ${font.height * upscale}`);
            nl();

            if (font.sizes) {
                for (let size of font.sizes) {
                    pathBase = `\${RENDER_FONT_DIR}/${font.name}${size.suffix}${upsfx}`;

                    buildDeps.push(`${pathBase}.yaml`);
                    buildDeps.push(`${pathBase}.png`);
        

                    outn(`build ${pathBase}.yaml ${pathBase}.png: fontmake \${FONTS_DIR}/${font.file}`);
                    outn(`    height = ${size.height * upscale}`);
                    nl();
                }
            }
        }
    }
    

    outn("#");
    outn("# Texture dependencies");
    outn("#");
    nl();

    for (let tex of textures_) {
        if (!params.nohpp) {
            outn(`build \${SRC_GEN_DIR}/${tex.name}.hpp: texmake_h ${tex.name}_desc.yaml | ${buildDeps.join(' ')}`);
        }

        for (let upscale of upscales_) {
            let upsfx = upscale === 1 ? "" : `-${upscale}x`;

            let basePath = `\${TEXTURES_DIR}/${tex.name}${upsfx}`;

            outn(`build ${basePath}.yaml ${basePath}.png: texmake${upsfx} ${tex.name}_desc.yaml | ${buildDeps.join(' ')}`);
        }

        nl();
    }


    nl();

    fs.writeFileSync(params.target, content_);

}

