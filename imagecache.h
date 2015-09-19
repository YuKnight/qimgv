#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "sourceContainers/imagestatic.h"
#include "sourceContainers/imageanimated.h"
#include "sourceContainers/video.h"
#include "sourceContainers/thumbnail.h"
#include "lib/imagelib.h"
#include "settings.h"
#include <QList>
#include <QtConcurrent>
#include <QMutex>
#include <ctime>

class CacheObject {
public:
    CacheObject(QString _path) : img(NULL), thumbnail(NULL), info(NULL), path(_path) {
        //possible slowdown here
        info = new FileInfo(path);
    }

    ~CacheObject() {
        delete img;
        delete info;
    }
    void generateThumbnail() {
        mutex.lock();
        delete thumbnail;
        thumbnail = new Thumbnail;
        thumbnail->image = getImg()->generateThumbnail();
        if(info->getType() == GIF) {
            thumbnail->label = "[gif]";
        } else if(info->getType() == VIDEO) {
                thumbnail->label = "[webm]";
        }
        if(thumbnail->image->size() == QSize(0,0)) {
            delete thumbnail->image;
            thumbnail->image = new QPixmap(":/images/res/error_no_image_100px.png");
        }
        thumbnail->name = info->getFileName();
        mutex.unlock();
    }
    const Thumbnail* getThumbnail() {
        if(!thumbnail) {
            generateThumbnail();
        }
        return const_cast<const Thumbnail*>(thumbnail);
    }
    const FileInfo* getInfo() {
        return const_cast<const FileInfo*>(getImg()->getInfo());
    }
    bool isLoaded() {
        if(img) {
            return img->isLoaded();
        }
        else return false;
    }
    void load() {
        mutex.lock();
        getImg()->load();
        info = getImg()->getInfo();
        mutex.unlock();
    }
    void unload() {
        mutex.lock();
        if(img) {
            img->unload();
        }
        mutex.unlock();
    }
    Image* image() {
        return img;
    }
private:
    void init() {
        if(info->getType() == GIF) {
            img = new ImageAnimated(info);
        } else if (info->getType() == VIDEO) {
            img = new Video(info);
        } else {
            img = new ImageStatic(info);
        }
    }
    Image* getImg() {
        if(!img) {
            init();
        }
        return img;
    }
    Image *img;
    Thumbnail *thumbnail;
    FileInfo *info;
    QString path;
    QMutex mutex;
};


class ImageCache : public QObject
{
    Q_OBJECT
public:
    ImageCache();
    ~ImageCache();
    void lock();
    void unlock();
    void loadAt(int pos);
    Image *imageAt(int pos);
    void init(QString dir, QStringList list);
    const FileInfo *infoAt(int pos);
    int length() const;
    void generateAllThumbnails();
    void unloadAll();
    QFuture<void> *future;
    void unloadAt(int pos);
    bool isLoaded(int pos);
    int currentlyLoadedCount();

signals:
    void initialized(int count);

public slots:
    const Thumbnail *thumbnailAt(int pos) const;
    QString currentDirectory();

private:
    QList<CacheObject*> *cachedImages;
    uint maxCacheSize;
    QString dir;
    QMutex mutex;
    void readSettings();

private slots:
    void applySettings();
};

#endif // IMAGECACHE_H
