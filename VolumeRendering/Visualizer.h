#ifndef VOLUME_RENDERING_VISUALIZER_H
#define VOLUME_RENDERING_VISUALIZER_H

#include <MediaWorkbench/GL/GlProgram.h>
#include <MediaWorkbench/GL/GlVao.h>

#include <CellarWorkbench/DataStructure/Matrix.h>

#include <PropRoom2D/Hud/TextHud.h>

#include <Scaena/Character/AbstractCharacter.h>

#include "Volumes.h"
#include "Lights.h"


class Visualizer :
        public scaena::AbstractCharacter
{
public:
    Visualizer(scaena::AbstractStage& stage);
    virtual ~Visualizer();

    virtual void enterStage();
    virtual void beginStep(const scaena::StageTime &time);
    virtual void draw(const scaena::StageTime &time);
    virtual void exitStage();

    virtual bool mousePressEvent(const scaena::MouseEvent &event);
    virtual bool mouseReleaseEvent(const scaena::MouseEvent &event);
    virtual bool mouseMoveEvent(const scaena::MouseEvent &event);

    virtual void updateMatrices();
    virtual void updateLightPos();

protected:
    virtual media::GlVbo3Df getBoxVertices(const cellar::Vec3f& from,
                                           const cellar::Vec3f& to);
    virtual void initVolumes();
    virtual void initCubeMap();

private:
    std::shared_ptr<prop2::TextHud> _fps;
    media::GlProgram _envRenderer;
    media::GlProgram _dataRenderer;
    media::GlVao _dataBox;
    media::GlVao _envBox;

    cellar::Vec3f _backgroundColor;
    cellar::Vec3i _dataSize;
    unsigned int _optTex;
    unsigned int _matTex;
    unsigned int _envTex;

    cellar::Mat4f _projection;
    cellar::Mat4f _view;
    cellar::Vec3f _eye;
    cellar::Vec3f _lgt;

    Shell _shell;
    Boil  _boil;
    SinNoise _sinNoise;
    BallFloor _ballFloor;
    IVolume& _volume;
    Light _light;

    bool _moveLight;
    bool _moveCamera;
};

#endif //VOLUME_RENDERING_VISUALIZER_H
