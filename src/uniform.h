#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "gl/fbo.h"
#include "gl/shader.h"
#include "gl/texture.h"

#include "scene/light.h"
#include "scene/camera.h"

#include "tools/fs.h"

struct UniformData {
    std::string getType();

    float   value[4];
    int     size;
    bool    bInt = false;
    bool    change = false;
};

typedef std::map<std::string, UniformData> UniformDataList;
// bool parseUniformData(const std::string &_line, UniformDataList *_uniforms);

struct UniformFunction {
    UniformFunction();
    UniformFunction(const std::string &_type);
    UniformFunction(const std::string &_type, std::function<void(Shader&)> _assign);
    UniformFunction(const std::string &_type, std::function<void(Shader&)> _assign, std::function<std::string()> _print);

    std::function<void(Shader&)>    assign;
    std::function<std::string()>    print;
    std::string                     type;
    bool                            present = false;
};

typedef std::map<std::string, UniformFunction> UniformFunctionsList;
typedef std::map<std::string, Texture*> TextureList;

class Uniforms {
public:
    Uniforms();
    virtual ~Uniforms();

    // Ingest new uniforms
    bool                    parseLine( const std::string &_line );

    bool                    addTexture( const std::string& _name, Texture* _texture );
    bool                    addTexture( const std::string& _name, const std::string& _path, WatchFileList& _files, bool _flip = true, bool _verbose = true );
    bool                    addBumpTexture( const std::string& _name, const std::string& _path, WatchFileList& _files, bool _flip = true, bool _verbose = true );

    void                    setCubeMap( TextureCube* _cubemap );
    void                    setCubeMap( const std::string& _filename, WatchFileList& _files, bool _verbose = true);
    
    // Check presence of uniforms on shaders
    void                    checkPresenceIn( const std::string &_vert_src, const std::string &_frag_src );

    // Feed uniforms to a specific shader
    bool                    feedTo( Shader &_shader );

    Camera&                 getCamera() { return cameras[0]; } 

    // Debug
    void                    print(bool _all);
    void                    printBuffers();
    void                    printTextures();
    void                    printLights();

    // Change state
    void                    flagChange();
    void                    unflagChange();
    bool                    haveChange();

    void                    clear();

    // Manually defined uniforms (through console IN)
    UniformDataList         data;

    // Automatic uniforms
    UniformFunctionsList    functions;

    // Common 
    TextureList             textures;
    TextureCube*            cubemap;
    std::vector<Fbo>        buffers;

    // 3d Scene Uniforms 
    std::vector<Camera>     cameras;
    std::vector<Light>      lights;

protected:
    bool                    m_change;
};


