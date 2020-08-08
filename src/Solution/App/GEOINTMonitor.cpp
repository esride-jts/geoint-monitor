
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "GEOINTMonitor.h"

#include "Basemap.h"
#include "IdentifyGraphicsOverlayResult.h"
#include "Map.h"
#include "MapQuickView.h"

#include "GdeltEventLayer.h"

#include <QDir>
#include <QUrl>

using namespace Esri::ArcGISRuntime;

GEOINTMonitor::GEOINTMonitor(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(Basemap::openStreetMap(this), this)),
    m_gdeltLayer(new GdeltEventLayer(this))
{
}

GEOINTMonitor::~GEOINTMonitor()
{
}

MapQuickView* GEOINTMonitor::mapView() const
{
    return m_mapView;
}

// Set the view (created in QML)
void GEOINTMonitor::setMapView(MapQuickView* mapView)
{
    if (!mapView || mapView == m_mapView)
    {
        return;
    }

    m_mapView = mapView;
    m_mapView->setMap(m_map);
    connect(m_mapView, &MapQuickView::exportImageCompleted, this, &GEOINTMonitor::exportMapImageCompleted);
    connect(m_mapView, &MapQuickView::mouseClicked, this, &GEOINTMonitor::mouseClicked);
    connect(m_mapView, &MapQuickView::identifyGraphicsOverlayCompleted, this, &GEOINTMonitor::identifyGraphicsOverlayCompleted);

    // Add the GDELT query layer
    GraphicsOverlay* gdeltOverlay = m_gdeltLayer->overlay();
    m_mapView->graphicsOverlays()->append(gdeltOverlay);

    emit mapViewChanged();
}

QString GEOINTMonitor::lastMapImageFilePath() const
{
    return m_lastMapImageFilePath;
}

void GEOINTMonitor::exportMapImage() const
{
    if (!m_mapView)
    {
        return;
    }

    m_mapView->exportImage();
}

void GEOINTMonitor::identifyGraphicsOverlayCompleted(QUuid taskId, Esri::ArcGISRuntime::IdentifyGraphicsOverlayResult* identifyResult)
{
    Q_UNUSED(taskId);
    if (!identifyResult->error().isEmpty())
    {
        return;
    }

    QList<Graphic*> identifiedGraphics = identifyResult->graphics();
    foreach (const Graphic* graphic, identifiedGraphics)
    {
        qDebug() << graphic;
        m_mapView->calloutData()->setTitle(graphic->attributes()->attributeValue("name").toString());
    }
}

void GEOINTMonitor::mouseClicked(QMouseEvent& mouseEvent)
{
    if (!m_mapView)
    {
        return;
    }

    const double pixelTolerance = 23;
    bool onlyPopups = false;
    GraphicsOverlay* gdeltOverlay = m_gdeltLayer->overlay();
    m_mapView->identifyGraphicsOverlay(gdeltOverlay, mouseEvent.x(), mouseEvent.y(), pixelTolerance, onlyPopups);
}

void GEOINTMonitor::queryGdelt() const
{
    m_gdeltLayer->query();
}

void GEOINTMonitor::exportMapImageCompleted(QUuid taskId, QImage image)
{
    Q_UNUSED(taskId);

    QDateTime now = QDateTime::currentDateTime();
    QString nowAsString = now.toString("yyyy-MM-dd_HH.mm.ss");
    QString fileName = "GEOINT-Monitor_" + nowAsString + ".png";
    QDir imageDir = QDir::temp();
    QString absoluteFileName = imageDir.absoluteFilePath(fileName);
    if (image.save(absoluteFileName))
    {
        // Emit map image exported
        m_lastMapImageFilePath = absoluteFileName;
        emit mapImageExported();
    }
}
