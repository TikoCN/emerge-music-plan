#ifndef SPECTRUMITEM_H
#define SPECTRUMITEM_H

#include <QColor>
#include <QQuickItem>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class QSGNode;

/**
 * @brief 使用 Qt Quick 场景图绘制实时音频频谱。
 */
class SpectrumItem : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList samples READ samples WRITE setSamples NOTIFY samplesChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged FINAL)

public:
    enum Mode {
        Waveform,
        Circular
    };
    Q_ENUM(Mode)

    /**
     * @brief 创建可由场景图绘制的频谱项。
     */
    explicit SpectrumItem(QQuickItem *parent = nullptr);

    /**
     * @brief 获取当前频谱样本。
     * @return 频谱样本列表。
     */
    [[nodiscard]] QVariantList samples() const;

    /**
     * @brief 设置当前频谱样本。
     * @param samples 归一化到 0 至 1 的频谱样本。
     */
    void setSamples(const QVariantList &samples);

    /**
     * @brief 获取频谱颜色。
     * @return 频谱颜色。
     */
    [[nodiscard]] QColor color() const;

    /**
     * @brief 设置频谱颜色。
     * @param color 新颜色。
     */
    void setColor(const QColor &color);

    /**
     * @brief 获取绘制模式。
     * @return 波形或环形频谱模式。
     */
    [[nodiscard]] Mode mode() const;

    /**
     * @brief 设置绘制模式。
     * @param mode 波形或环形频谱模式。
     */
    void setMode(Mode mode);

    /**
     * @brief 获取环形频谱的基准半径。
     * @return 基准半径。
     */
    [[nodiscard]] qreal radius() const;

    /**
     * @brief 设置环形频谱的基准半径。
     * @param radius 基准半径。
     */
    void setRadius(qreal radius);

    /**
     * @brief 获取环形频谱的起始角度。
     * @return 起始角度，单位为度。
     */
    [[nodiscard]] qreal startAngle() const;

    /**
     * @brief 设置环形频谱的起始角度。
     * @param startAngle 起始角度，单位为度。
     */
    void setStartAngle(qreal startAngle);

signals:
    void samplesChanged();
    void colorChanged();
    void modeChanged();
    void radiusChanged();
    void startAngleChanged();

protected:
    /**
     * @brief 在场景图渲染线程更新频谱几何。
     * @param oldNode 上一帧的场景图节点。
     * @param data 场景图节点更新信息。
     * @return 更新后的场景图节点。
     * @override
     */
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    QVariantList m_samples;
    QColor m_color = Qt::white;
    Mode m_mode = Waveform;
    qreal m_radius = 0.0;
    qreal m_startAngle = 0.0;
    int m_nodeMode = -1;
};

#endif
