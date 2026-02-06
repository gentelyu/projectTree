#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 槽函数：响应“新建工程”菜单点击，添加新工程到工程树
    void onAddNewProject();

    // 右键菜单触发（核心，构建右键菜单）
    void onCustomContextMenuRequested(const QPoint &pos);

    // 右键菜单 - 关闭工程
    void onCloseProject();

    // 右键菜单 - 重命名工程
    void onRenameProject();

    // 右键菜单 - 保存工程
    void onSaveProject();

    // 右键菜单 - 打开工程所在位置
    void onOpenProjectLocation();

    // 预留槽函数（其他菜单选项，后续可扩展实现）
    void onNewSchematic();    // 新建原理图
    void onNewPcb();          // 新建PCB
    void onAddExistingFile(); // 添加已有文件
    void onCloseAllProjectFiles(); // 关闭工程所有文件
    void onSaveProjectAs();   // 工程另存为

    // ===== 下属节点（文件/分类）右键菜单槽函数 =====
    void onSaveFile();          // 保存文件
    void onSaveFileAs();        // 文件另存为
    void onRemoveFileFromProject(); // 从工程删除
    void onCloseFile();         // 关闭文件
    void onRenameFile();        // 重命名文件
    void onOpenFileLocation();  // 打开文件所在位置


private:
    // 1. 工程树核心组件
    QTreeView *m_projectTreeView;          // 工程树视图（仅负责展示）
    QStandardItemModel *m_projectItemModel; // 工程树数据模型（仅负责存储数据）

    // 保存当前右键点击的工程节点（用于菜单功能调用）
    QStandardItem *m_currentClickedProjectItem;

    // 保存当前右键点击的下属节点（文件/分类）
    QStandardItem *m_currentClickedSubItem;

    // 2. 辅助函数：初始化工程树（视图+模型）
    void initProjectTree();

    // 3. 辅助函数：添加新工程到工程树（核心方法，实现动态添加子工程）
    // projectName：工程名称（可后续替换为.pro文件解析后的名称）
    QStandardItem* addNewProject(const QString &projectName);

    // 辅助函数：获取工程默认模拟路径（支撑“打开所在位置”功能）
    QString getProjectDefaultPath(const QString &projectName);

    // 给下属节点生成模拟文件路径（支撑“打开文件所在位置”等功能）
    QString getSubItemDefaultFilePath(QStandardItem *projectItem, QStandardItem *subItem);

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
