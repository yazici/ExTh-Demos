#ifndef RAYTRACERGUI_H
#define RAYTRACERGUI_H

#include <QMainWindow>

#include <Scaena/Play/Play.h>

#include "PostProdManager.h"
#include "RaytracedView.h"

namespace Ui
{
    class RaytracerGui;
}

class RaytracerGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit RaytracerGui(const std::shared_ptr<scaena::Play>& play);

    ~RaytracerGui();

private:
    Ui::RaytracerGui* _ui;
    std::shared_ptr<scaena::Play> _play;
    std::shared_ptr<RaytracedView> _raytracedView;
    std::shared_ptr<PostProdManager> _postProdManager;
};

#endif // RAYTRACERGUI_H