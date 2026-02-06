#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

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

private:
    Ui::MainWindow *ui;

    // 总工程树控件
    QTreeWidget *m_totalProjectTree;

    // 辅助函数：创建一个新的工程树节点（子工程）
    QTreeWidgetItem* createNewProjectTree(const QString &projectName);
};
#endif // MAINWINDOW_H
