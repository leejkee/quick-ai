import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// 这是一个组合组件，包含左侧列表和右侧详情页
RowLayout {
    id: root
    spacing: 0

    property string providerId: ""

    property var currentProvider: {
        if (!configManager || !providerId) return null
        for (let i = 0; i < configManager.providers.length; i++) {
            if (configManager.providers[i].name === providerId) {
                return configManager.providers[i]
            }
        }
        return null
    }

    ListView {
        id: modelList
        Layout.preferredWidth: 200
        Layout.fillHeight: true
        clip: true
        color: "#f0f0f0"

        model: currentProvider ? currentProvider.models : []

        delegate: ItemDelegate {
            width: parent.width
            text: modelData.name
            highlighted: ListView.isCurrentItem
            onClicked: modelList.currentIndex = index
        }

        footer: Button {
            width: parent.width
            text: "+ Add Model"
            onClicked: {
                configManager.addDefaultModel(root.currentProviderId)
                modelList.currentIndex = modelList.count - 1
            }
        }
    }

    ModelDetailPage {
        Layout.fillWidth: true
        Layout.fillHeight: true

        // 1. 传入 Provider ID (用于保存时的定位)
        providerId: root.currentProviderId

        // 2. 传入当前选中的 Model 对象 (核心)
        // 当 ListView 切换 index，或者后端数据更新时，这个 modelData 会自动更新
        modelData: (root.currentProvider && modelList.currentIndex >= 0)
            ? root.currentProvider.models[modelList.currentIndex]
            : null
    }

}