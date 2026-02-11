cmake_minimum_required(VERSION 3.16)

# 先定义项目信息（规范写法）
project(TikoLib VERSION 0.1 LANGUAGES CXX)

# 设置 C++ 标准和 Qt 自动处理
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)  # 推荐用大写，和 Qt 官方保持一致

# 查找 Qt 依赖
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)  # 增加这行，确保 Qt 构建规范

# 定义库目标（名称和项目名一致，避免混淆）
add_library(TikoLib STATIC)  # 纯 QML 模块用 STATIC 库即可

# 标记单例 QML 文件
set_source_files_properties(TikoSeit.qml
        PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

# 关键策略：允许 QML 模块关联到静态库
qt_policy(SET QTP0004 NEW)

# 添加 QML 模块（合并重复的 QML_FILES，更整洁）
qt_add_qml_module(TikoLib
        URI Tiko                  # 模块 URI，主项目通过这个导入
        VERSION 1.0               # 版本号
        QML_FILES
        TikoSeit.qml
        TikoFrameless.qml

        bar/TikoBarV.qml
        button/TikoButtonBorder.qml
        button/TikoButtonNormal.qml
        button/TikoButtonBase.qml
        button/TikoButtonCombox.qml
        button/TikoButtonSwitch.qml
        button/TikoButtonIcon.qml
        text/TikoTextInput.qml
        text/TikoTextLine.qml
        menu/TikoMenu.qml
        menu/TikoMenuItem.qml
        menu/TikoMenuSpeacer.qml
        message/TikoMessageLittle.qml
        progress/TikoProgressBar.qml
        progress/TikoProgressRing.qml
        popup/TikoPopup.qml
        popup/TikoPopupInput.qml
        popup/TikoPopupMessage.qml
        slider/TikoSliderH.qml
        slider/TikoSliderV.qml
        vessel/TikoRightVessel.qml
        select/TikoSelectColor.qml
        select/TikoSelectFont.qml
        image/TikoImageAuto.qml
        image/TikoImage.qml
        QML_FILES button/TikoButtonText.qml
        QML_FILES button/TikoButtonComplete.qml
        dynamic/TikoDynamicTextLine.qml
        dynamic/TikoDynamicIcon.qml
        dynamic/TikoDynamicBackGround.qml
        dynamic/TikoDynamicColor.qml
        dynamic/TikoDynamicState.qml
        QML_FILES text/TikoTextLineType.qml
        QML_FILES text/TikoTextTitle.qml
        QML_FILES text/TikoTextSubtitle.qml
        QML_FILES text/TikoTextInformation.qml
        QML_FILES text/TikoTextAssistance.qml
        QML_FILES TikoTheme.qml
        QML_FILES vessel/TikoThemeBackground.qml
)

# 链接 Qt 依赖（纯 QML 模块也需要链接 QtQuick）
target_link_libraries(TikoLib PRIVATE Qt6::Quick)

# 安装模块（可选，方便主项目查找）
install(TARGETS TikoLib
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
