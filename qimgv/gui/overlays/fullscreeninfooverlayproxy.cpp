#include "fullscreeninfooverlayproxy.h"

FullscreenInfoOverlayProxy::FullscreenInfoOverlayProxy(FloatingWidgetContainer *parent)
    : container(parent),
      infoOverlay(nullptr)
{
}

FullscreenInfoOverlayProxy::~FullscreenInfoOverlayProxy() {
    if(infoOverlay)
        infoOverlay->deleteLater();
}

void FullscreenInfoOverlayProxy::show() {
    init();
    infoOverlay->show();
}

void FullscreenInfoOverlayProxy::hide() {
    if(infoOverlay)
        infoOverlay->hide();
}

void FullscreenInfoOverlayProxy::setInfo(QString _position, QString _fileName, QString _info) {
    if(infoOverlay) {
        infoOverlay->setInfo(_position, _fileName, _info);
    } else {
        stateBuf.position = _position;
        stateBuf.fileName = _fileName;
        stateBuf.info = _info;
    }
}

void FullscreenInfoOverlayProxy::init() {
    if(infoOverlay)
        return;
    infoOverlay = new FullscreenInfoOverlay(container);
    if(!stateBuf.fileName.isEmpty())
        setInfo(stateBuf.position, stateBuf.fileName, stateBuf.info);
}
