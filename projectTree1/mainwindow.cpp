#include "mainwindow.h"
#include "./ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // 1. 设置窗口基本属性
    this->setWindowTitle("多工程树管理器");
    this->resize(1000, 600);

    // 2. 创建中心部件和布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    this->setCentralWidget(centralWidget);

    // 3. 初始化总工程树
    m_totalProjectTree = new QTreeWidget(this);
    // 设置总树的列数（树形控件默认支持多列，这里只需要1列）
    m_totalProjectTree->setColumnCount(1);
    // 设置总树的表头标题
    m_totalProjectTree->setHeaderLabel("工程管理器");
    // 可选：隐藏表头（让界面更简洁，只显示树结构）
    // m_totalProjectTree->setHeaderHidden(true);

    // 4. 将总树添加到布局中
    centralLayout->addWidget(m_totalProjectTree);

    // 5. 启动程序时，默认创建一个示例工程（子工程树）
    QTreeWidgetItem *defaultProject = createNewProjectTree("默认工程_1");
    // 将默认工程添加到总工程树中（作为总树的一级节点，即子工程树的根）
    m_totalProjectTree->addTopLevelItem(defaultProject);
    defaultProject->setIcon(0, QIcon(":/icon.png"));
    // 可选：默认展开这个工程树
    defaultProject->setExpanded(true);

    QTreeWidgetItem *defaultProject2 = createNewProjectTree("默认工程_2");
    // 将默认工程添加到总工程树中（作为总树的一级节点，即子工程树的根）
    m_totalProjectTree->addTopLevelItem(defaultProject2);
    defaultProject2->setIcon(0, QIcon(":/icon.png"));
    // 可选：默认展开这个工程树
    defaultProject2->setExpanded(true);


    // 6. 添加菜单栏，实现新建工程功能
    QMenu *fileMenu = this->menuBar()->addMenu("文件");
    QAction *newProjectAction = new QAction("新建工程", this);
    fileMenu->addAction(newProjectAction);

    // 绑定信号槽：点击“新建工程”时，创建新的子工程树
    connect(newProjectAction, &QAction::triggered, this, [=]() {
        static int projectIndex = 3; // 从3开始，因为默认工程是1 2
        QString projectName = QString("新工程_%1").arg(projectIndex);
        QTreeWidgetItem *newProject = createNewProjectTree(projectName);
        m_totalProjectTree->addTopLevelItem(newProject);
        newProject->setExpanded(true);
        projectIndex++;
    });





    // 在MainWindow类中添加右键菜单的处理逻辑
    // 先在构造函数中开启总树的右键菜单支持
    m_totalProjectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_totalProjectTree, &QTreeWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        // 获取右键点击的节点
        QTreeWidgetItem *clickedItem = m_totalProjectTree->itemAt(pos);
        if (clickedItem == nullptr) return; // 点击空白处，不处理

        // 判断是否是子工程树根节点（一级节点）
        QTreeWidgetItem *parentItem = clickedItem->parent();
        if (parentItem == nullptr) { // 一级节点（子工程树根）
            QMenu menu(this);
            QAction *closeProjectAction = new QAction("关闭工程", &menu);
            menu.addAction(closeProjectAction);

            // 绑定关闭工程的信号槽
            connect(closeProjectAction, &QAction::triggered, this, [=]() {
                // 删除该子工程树节点
                int index = m_totalProjectTree->indexOfTopLevelItem(clickedItem);
                m_totalProjectTree->takeTopLevelItem(index);
                delete clickedItem; // 释放内存
            });

            // 显示右键菜单
            menu.exec(m_totalProjectTree->mapToGlobal(pos));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTreeWidgetItem *MainWindow::createNewProjectTree(const QString &projectName)
{
    // 1. 创建子工程树的根节点（对应一个工程）
    QTreeWidgetItem *projectRoot = new QTreeWidgetItem();
    // 设置工程名称（显示在树节点上）
    projectRoot->setText(0, projectName);
    // 可选：给工程节点设置一个图标（需要提前准备图标资源）
    projectRoot->setIcon(0, QIcon(":/icon.png"));

    // 2. 给子工程树添加默认的子节点（模拟工程的目录结构）
    // 2.1 创建“源文件”节点
    QTreeWidgetItem *sourceFiles = new QTreeWidgetItem(projectRoot);
    sourceFiles->setText(0, "源文件");
    // 给源文件添加一个示例文件
    QTreeWidgetItem *mainCpp = new QTreeWidgetItem(sourceFiles);
    mainCpp->setText(0, "main.cpp");

    // 2.2 创建“头文件”节点
    QTreeWidgetItem *headerFiles = new QTreeWidgetItem(projectRoot);
    headerFiles->setText(0, "头文件");
    QTreeWidgetItem *mainH = new QTreeWidgetItem(headerFiles);
    mainH->setText(0, "main.h");

    // 2.3 创建“资源文件”节点
    QTreeWidgetItem *resFiles = new QTreeWidgetItem(projectRoot);
    resFiles->setText(0, "资源文件");
    QTreeWidgetItem *qrcFile = new QTreeWidgetItem(resFiles);
    qrcFile->setText(0, "resource.qrc");

    // 3. 返回创建好的子工程树根节点
    return projectRoot;
}
