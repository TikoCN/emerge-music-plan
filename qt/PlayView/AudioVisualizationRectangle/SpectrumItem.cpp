#include "SpectrumItem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGNode>
#include <algorithm>
#include <cmath>

namespace {

constexpr int circleSegments = 96;
constexpr qreal pi = 3.14159265358979323846;

/**
 * @brief 创建带独立几何和颜色材质的场景图节点。
 * @param vertexCount 几何顶点数量。
 * @param drawingMode 顶点的绘制方式。
 * @param color 节点颜色。
 * @return 已初始化的几何节点。
 */
QSGGeometryNode *createGeometryNode(const int vertexCount, const QSGGeometry::DrawingMode drawingMode,
                                    const QColor &color) {
    auto *node = new QSGGeometryNode;
    auto *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);
    geometry->setDrawingMode(drawingMode);
    geometry->setVertexDataPattern(QSGGeometry::DynamicPattern);
    geometry->setLineWidth(1.5f);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    auto *material = new QSGFlatColorMaterial;
    material->setColor(color);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);
    return node;
}

/**
 * @brief 更新动态几何节点的顶点数量和颜色。
 * @param node 要更新的几何节点。
 * @param vertexCount 新顶点数量。
 * @param color 新颜色。
 */
void prepareGeometryNode(QSGGeometryNode *node, const int vertexCount, const QColor &color) {
    QSGGeometry *geometry = node->geometry();
    if (geometry->vertexCount() != vertexCount) {
        geometry->allocate(vertexCount);
    }
    static_cast<QSGFlatColorMaterial *>(node->material())->setColor(color);
}

/**
 * @brief 将频谱样本限制在绘图使用的归一化范围内。
 * @param samples 频谱样本。
 * @param index 样本索引。
 * @return 0 至 1 之间的样本值。
 */
qreal sampleValue(const QVariantList &samples, const int index) {
    return std::clamp(samples[index].toReal(), 0.0, 1.0);
}

}

SpectrumItem::SpectrumItem(QQuickItem *parent)
    : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

QVariantList SpectrumItem::samples() const {
    return m_samples;
}

void SpectrumItem::setSamples(const QVariantList &samples) {
    if (m_samples == samples) {
        return;
    }
    m_samples = samples;
    emit samplesChanged();
    update();
}

QColor SpectrumItem::color() const {
    return m_color;
}

void SpectrumItem::setColor(const QColor &color) {
    if (m_color == color) {
        return;
    }
    m_color = color;
    emit colorChanged();
    update();
}

SpectrumItem::Mode SpectrumItem::mode() const {
    return m_mode;
}

void SpectrumItem::setMode(const Mode mode) {
    if (m_mode == mode) {
        return;
    }
    m_mode = mode;
    emit modeChanged();
    update();
}

qreal SpectrumItem::radius() const {
    return m_radius;
}

void SpectrumItem::setRadius(const qreal radius) {
    if (qFuzzyCompare(m_radius, radius)) {
        return;
    }
    m_radius = radius;
    emit radiusChanged();
    update();
}

qreal SpectrumItem::startAngle() const {
    return m_startAngle;
}

void SpectrumItem::setStartAngle(const qreal startAngle) {
    if (qFuzzyCompare(m_startAngle, startAngle)) {
        return;
    }
    m_startAngle = startAngle;
    emit startAngleChanged();
    update();
}

QSGNode *SpectrumItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    const int sampleCount = m_samples.size();
    const int segmentCount = std::max(0, sampleCount - 1);
    const int fillVertexCount = m_mode == Waveform ? segmentCount * 6 : 0;
    const int lineVertexCount = m_mode == Circular ? sampleCount * 2 : segmentCount * 2;
    const int ringVertexCount = m_mode == Circular ? circleSegments * 2 : 0;

    auto *root = static_cast<QSGNode *>(oldNode);
    if (root != nullptr && m_nodeMode != m_mode) {
        delete root;
        root = nullptr;
    }
    if (root == nullptr) {
        root = new QSGNode;
        root->appendChildNode(createGeometryNode(
            fillVertexCount, QSGGeometry::DrawTriangles,
            QColor(m_color.red(), m_color.green(), m_color.blue(), 71)));
        root->appendChildNode(createGeometryNode(lineVertexCount, QSGGeometry::DrawLines, m_color));
        root->appendChildNode(createGeometryNode(
            ringVertexCount, QSGGeometry::DrawLines,
            QColor(m_color.red(), m_color.green(), m_color.blue(), 89)));
        m_nodeMode = m_mode;
    }

    auto *fillNode = static_cast<QSGGeometryNode *>(root->firstChild());
    auto *lineNode = static_cast<QSGGeometryNode *>(fillNode->nextSibling());
    auto *ringNode = static_cast<QSGGeometryNode *>(lineNode->nextSibling());
    const QColor fillColor(m_color.red(), m_color.green(), m_color.blue(), qRound(m_color.alpha() * 0.28));
    const QColor ringColor(m_color.red(), m_color.green(), m_color.blue(), qRound(m_color.alpha() * 0.35));
    prepareGeometryNode(fillNode, fillVertexCount, fillColor);
    prepareGeometryNode(lineNode, lineVertexCount, m_color);
    prepareGeometryNode(ringNode, ringVertexCount, ringColor);

    if (sampleCount == 0 || width() <= 0 || height() <= 0) {
        fillNode->geometry()->markVertexDataDirty();
        lineNode->geometry()->markVertexDataDirty();
        ringNode->geometry()->markVertexDataDirty();
        fillNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        lineNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        ringNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        return root;
    }

    if (m_mode == Circular) {
        const qreal centerX = width() / 2.0;
        const qreal centerY = height() / 2.0;
        const qreal baseRadius = m_radius > 0.0 ? m_radius : std::min(width(), height()) * 0.3;
        const qreal maxAmplitude = baseRadius * 0.45;
        const qreal angleStep = 2.0 * pi / sampleCount;
        auto *lineVertices = lineNode->geometry()->vertexDataAsPoint2D();
        auto *ringVertices = ringNode->geometry()->vertexDataAsPoint2D();

        for (int i = 0; i < sampleCount; ++i) {
            const qreal angle = i * angleStep + m_startAngle * pi / 180.0;
            const qreal value = sampleValue(m_samples, i);
            const qreal outerRadius = baseRadius + value * maxAmplitude;
            lineVertices[i * 2].set(centerX + std::cos(angle) * baseRadius,
                                    centerY + std::sin(angle) * baseRadius);
            lineVertices[i * 2 + 1].set(centerX + std::cos(angle) * outerRadius,
                                        centerY + std::sin(angle) * outerRadius);
        }

        for (int i = 0; i < circleSegments; ++i) {
            const qreal firstAngle = 2.0 * pi * i / circleSegments;
            const qreal secondAngle = 2.0 * pi * (i + 1) / circleSegments;
            ringVertices[i * 2].set(centerX + std::cos(firstAngle) * baseRadius,
                                    centerY + std::sin(firstAngle) * baseRadius);
            ringVertices[i * 2 + 1].set(centerX + std::cos(secondAngle) * baseRadius,
                                        centerY + std::sin(secondAngle) * baseRadius);
        }
    } else {
        auto *lineVertices = lineNode->geometry()->vertexDataAsPoint2D();
        auto *fillVertices = fillNode->geometry()->vertexDataAsPoint2D();
        for (int i = 0; i < segmentCount; ++i) {
            const qreal x1 = i * width() / segmentCount;
            const qreal x2 = (i + 1) * width() / segmentCount;
            const qreal y1 = height() - height() * 0.9 * sampleValue(m_samples, i);
            const qreal y2 = height() - height() * 0.9 * sampleValue(m_samples, i + 1);
            lineVertices[i * 2].set(x1, y1);
            lineVertices[i * 2 + 1].set(x2, y2);

            const int vertex = i * 6;
            fillVertices[vertex].set(x1, height());
            fillVertices[vertex + 1].set(x1, y1);
            fillVertices[vertex + 2].set(x2, y2);
            fillVertices[vertex + 3].set(x1, height());
            fillVertices[vertex + 4].set(x2, y2);
            fillVertices[vertex + 5].set(x2, height());
        }
    }

    fillNode->geometry()->markVertexDataDirty();
    lineNode->geometry()->markVertexDataDirty();
    ringNode->geometry()->markVertexDataDirty();
    fillNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    lineNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    ringNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return root;
}
