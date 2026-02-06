#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_currentClickedProjectItem(nullptr)
    , m_currentClickedSubItem(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 窗口基础配置
    this->setWindowTitle("QTreeView 工程树管理器");
    this->resize(1000, 600);

    // 1. 初始化工程树（视图+模型）
    initProjectTree();

    // 2. 添加菜单栏：提供“新建工程”入口，方便测试动态添加功能
    QMenu *fileMenu = this->menuBar()->addMenu("文件");
    QAction *newProjectAction = new QAction("新建工程", this);
    fileMenu->addAction(newProjectAction);

    // 3. 绑定信号槽：点击“新建工程”触发添加工程逻辑
    connect(newProjectAction, &QAction::triggered, this, &MainWindow::onAddNewProject);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initProjectTree()
{
    // 1. 创建中心部件和布局（承载QTreeView）
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    this->setCentralWidget(centralWidget);

    // 2. 初始化数据模型（工程树的核心数据容器）
    m_projectItemModel = new QStandardItemModel(this);
    // 设置模型列数（树形结构默认1列即可，多列可用于展示文件大小、修改时间等附加信息）
    m_projectItemModel->setColumnCount(1);
    // 设置表头标题（可隐藏，这里保留让界面更清晰）
    m_projectItemModel->setHeaderData(0, Qt::Horizontal, "工程管理器");

    // 3. 初始化视图（仅负责展示模型中的数据）
    m_projectTreeView = new QTreeView(this);
    // 绑定视图与模型（核心步骤：视图展示模型的数据）
    m_projectTreeView->setModel(m_projectItemModel);
    // 界面优化：让工程树结构更清晰
    m_projectTreeView->setHeaderHidden(false); // 显示表头（如需隐藏设为true）
    m_projectTreeView->setRootIsDecorated(true); // 显示根节点的展开/折叠图标
    m_projectTreeView->setItemsExpandable(true); // 节点支持展开/折叠
    m_projectTreeView->setSelectionBehavior(QTreeView::SelectItems); // 选中整个节点

    // 开启右键菜单支持（核心配置）
    m_projectTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    // 绑定右键菜单触发信号到槽函数
    connect(m_projectTreeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);

    // 4. 将视图添加到布局中
    centralLayout->addWidget(m_projectTreeView);
}


// 核心方法：添加新工程到工程树
QStandardItem *MainWindow::addNewProject(const QString &projectName)
{
    // 1. 校验工程名称（避免空名称）
    QString validProjectName = projectName.trimmed();
    if (validProjectName.isEmpty())
    {
        validProjectName = "未命名工程";
    }

    // 2. 创建工程根节点（QStandardItem 对应模型中的一个节点）
    QStandardItem *projectRootItem = new QStandardItem(validProjectName);
    // 节点优化：设置工程节点不可编辑（如需重命名可设为true）
    projectRootItem->setEditable(false);
    // 可选：设置工程节点图标（后续可替换为自定义工程图标，需提前准备资源文件）
    projectRootItem->setIcon(QIcon(":/icon.png"));
    // 可选：设置节点提示信息（鼠标悬浮时显示）
    projectRootItem->setToolTip(QString("工程名称：%1\n工程路径：%2")
                                    .arg(validProjectName, getProjectDefaultPath(validProjectName)));

    // 新增：给工程节点存储隐藏数据 - 工程路径（支撑“打开所在位置”功能）
    // Qt::UserRole 用于存储自定义隐藏数据，避免与系统内置角色冲突
    projectRootItem->setData(getProjectDefaultPath(validProjectName), Qt::UserRole + 1);

    // 3. 将工程节点添加到模型中（作为一级节点，即根工程树的子节点）
    // 注：QStandardItemModel 默认有一个隐形根节点，appendRow 是给隐形根节点添加子节点（即工程树的一级节点）
    m_projectItemModel->appendRow(projectRootItem);

    // 4. 可选：给新工程添加默认子节点（演示结构，后续可替换为.pro文件解析的文件列表）
    QStandardItem *sourceFilesItem = new QStandardItem("源文件");
    QStandardItem *headerFilesItem = new QStandardItem("头文件");
    QStandardItem *resFilesItem = new QStandardItem("资源文件");
    sourceFilesItem->setEditable(false);
    headerFilesItem->setEditable(false);
    resFilesItem->setEditable(false);

    // 新增：给下属分类节点添加示例文件节点（更贴近实际工程，方便测试右键菜单）
    QStandardItem *mainCppItem = new QStandardItem("main.cpp");
    QStandardItem *mainHItem = new QStandardItem("main.h");
    QStandardItem *resourceQrcItem = new QStandardItem("resource.qrc");
    mainCppItem->setEditable(false);
    mainHItem->setEditable(false);
    resourceQrcItem->setEditable(false);

    // 给分类节点添加文件节点
    sourceFilesItem->appendRow(mainCppItem);
    headerFilesItem->appendRow(mainHItem);
    resFilesItem->appendRow(resourceQrcItem);

    // 5. 给下属节点存储隐藏数据（文件路径），支撑右键菜单功能
    QList<QStandardItem*> subItems = {sourceFilesItem, headerFilesItem, resFilesItem,
                                       mainCppItem, mainHItem, resourceQrcItem};
    foreach (QStandardItem *subItem, subItems)
    {
        QString subItemPath = getSubItemDefaultFilePath(projectRootItem, subItem);
        subItem->setData(subItemPath, Qt::UserRole + 2); // 用 UserRole+2 区分工程路径，避免冲突
        subItem->setToolTip(QString("文件名称：%1\n文件路径：%2").arg(subItem->text(), subItemPath));
    }


    // 将子节点添加到工程根节点下
    projectRootItem->appendRow(sourceFilesItem);
    projectRootItem->appendRow(headerFilesItem);
    projectRootItem->appendRow(resFilesItem);

    // 5. 视图自动展开新添加的工程节点（提升用户体验）
    m_projectTreeView->expand(projectRootItem->index());

    // 6. 返回工程根节点（方便后续给该工程添加更多子文件节点）
    return projectRootItem;
}

// 新增：获取工程默认模拟路径（后续可替换为真实.pro文件路径）
QString MainWindow::getProjectDefaultPath(const QString &projectName)
{
    // 模拟系统桌面路径下的工程目录（QT跨平台兼容）
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString projectPath = QString("%1/%2_Project").arg(desktopPath, projectName);
    // 模拟创建目录（仅用于演示，实际工程需判断目录是否存在）
    QDir().mkpath(projectPath);
    return projectPath;
}

// 新增：给下属节点生成模拟文件路径
QString MainWindow::getSubItemDefaultFilePath(QStandardItem *projectItem, QStandardItem *subItem)
{
    if (projectItem == nullptr || subItem == nullptr) return "";

    // 1. 获取工程路径
    QString projectPath = projectItem->data(Qt::UserRole + 1).toString();
    if (projectPath.isEmpty()) return "";

    // 2. 构建下属节点文件路径
    QString subItemName = subItem->text();
    QString subItemPath = projectPath;

    // 区分分类节点和文件节点
    if (subItemName == "源文件" || subItemName == "头文件" || subItemName == "资源文件")
    {
        // 分类节点：路径为工程目录下的子目录
        subItemPath += QString("/%1").arg(subItemName);
    }
    else
    {
        // 文件节点：路径为对应分类目录下的文件
        QStandardItem *parentSubItem = subItem->parent();
        if (parentSubItem != nullptr)
        {
            subItemPath += QString("/%1/%2").arg(parentSubItem->text(), subItemName);
        }
        else
        {
            subItemPath += QString("/%1").arg(subItemName);
        }
    }

    // 3. 模拟创建目录（分类节点）或空文件（文件节点）
    QDir dir;
    if (subItemName == "源文件" || subItemName == "头文件" || subItemName == "资源文件")
    {
        dir.mkpath(subItemPath);
    }
    else
    {
        dir.mkpath(QFileInfo(subItemPath).path()); // 创建文件所在目录
        QFile file(subItemPath);
        if (!file.exists()) // 创建空文件
        {
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }

    return subItemPath;
}


// 槽函数：响应“新建工程”菜单点击
void MainWindow::onAddNewProject()
{
    // 静态变量：用于生成递增的工程名称，方便测试
    static int projectIndex = 1;
    QString projectName = QString("新工程_%1").arg(projectIndex);

    // 调用添加工程方法
    addNewProject(projectName);

    // 工程索引自增
    projectIndex++;
}

// 核心槽函数：右键菜单触发，构建菜单界面
void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    // 1. 重置当前点击的工程节点
    m_currentClickedProjectItem = nullptr;
    m_currentClickedSubItem = nullptr;

    // 2. 获取右键点击的模型索引（对应视图中的节点）
    QModelIndex clickedIndex = m_projectTreeView->indexAt(pos);
    if (!clickedIndex.isValid()) return; // 点击空白处，不处理

    // 3. 获取对应的模型节点（QStandardItem）
    QStandardItem *clickedItem = m_projectItemModel->itemFromIndex(clickedIndex);
    if (clickedItem == nullptr) return;

    // 4. 区分节点类型，构建不同的右键菜单
    QStandardItem *parentItem = clickedItem->parent();
    if (parentItem == nullptr)
    {
        // ===== 情况1：工程根节点（无父节点）=====
        m_currentClickedProjectItem = clickedItem;

        // 构建工程根节点右键菜单（原有逻辑，保持不变）
        QMenu contextMenu(this);
        QAction *newSchematicAction = contextMenu.addAction("新建原理图");
        QAction *newPcbAction = contextMenu.addAction("新建PCB");
        contextMenu.addSeparator();
        QAction *addExistingFileAction = contextMenu.addAction("添加已有文件");
        contextMenu.addSeparator();
        QAction *closeProjectAction = contextMenu.addAction("关闭工程");
        QAction *closeAllFilesAction = contextMenu.addAction("关闭工程所有文件");
        contextMenu.addSeparator();
        QAction *saveProjectAction = contextMenu.addAction("保存工程");
        QAction *saveProjectAsAction = contextMenu.addAction("工程另存为");
        contextMenu.addSeparator();
        QAction *renameProjectAction = contextMenu.addAction("重命名");
        contextMenu.addSeparator();
        QAction *openLocationAction = contextMenu.addAction("打开工程所在位置");

        // 绑定信号槽
        connect(newSchematicAction, &QAction::triggered, this, &MainWindow::onNewSchematic);
        connect(newPcbAction, &QAction::triggered, this, &MainWindow::onNewPcb);
        connect(addExistingFileAction, &QAction::triggered, this, &MainWindow::onAddExistingFile);
        connect(closeProjectAction, &QAction::triggered, this, &MainWindow::onCloseProject);
        connect(closeAllFilesAction, &QAction::triggered, this, &MainWindow::onCloseAllProjectFiles);
        connect(saveProjectAction, &QAction::triggered, this, &MainWindow::onSaveProject);
        connect(saveProjectAsAction, &QAction::triggered, this, &MainWindow::onSaveProjectAs);
        connect(renameProjectAction, &QAction::triggered, this, &MainWindow::onRenameProject);
        connect(openLocationAction, &QAction::triggered, this, &MainWindow::onOpenProjectLocation);

        // 显示菜单
        contextMenu.exec(m_projectTreeView->mapToGlobal(pos));
    }
    else
    {
        // ===== 情况2：下属节点（有父节点，文件/分类）=====
        m_currentClickedSubItem = clickedItem;

        // 构建下属节点右键菜单（新增核心逻辑）
        QMenu contextMenu(this);

        // 菜单选项：保存、另存为、从工程删除、关闭、重命名、打开所在位置
        QAction *saveFileAction = contextMenu.addAction("保存");
        QAction *saveFileAsAction = contextMenu.addAction("另存为");
        contextMenu.addSeparator();
        QAction *removeFileAction = contextMenu.addAction("从工程中删除");
        contextMenu.addSeparator();
        QAction *closeFileAction = contextMenu.addAction("关闭");
        contextMenu.addSeparator();
        QAction *renameFileAction = contextMenu.addAction("重命名");
        contextMenu.addSeparator();
        QAction *openFileLocationAction = contextMenu.addAction("打开文件所在位置");

        // 绑定信号槽（新增下属节点槽函数）
        connect(saveFileAction, &QAction::triggered, this, &MainWindow::onSaveFile);
        connect(saveFileAsAction, &QAction::triggered, this, &MainWindow::onSaveFileAs);
        connect(removeFileAction, &QAction::triggered, this, &MainWindow::onRemoveFileFromProject);
        connect(closeFileAction, &QAction::triggered, this, &MainWindow::onCloseFile);
        connect(renameFileAction, &QAction::triggered, this, &MainWindow::onRenameFile);
        connect(openFileLocationAction, &QAction::triggered, this, &MainWindow::onOpenFileLocation);

        // 显示菜单
        contextMenu.exec(m_projectTreeView->mapToGlobal(pos));
    }
}

// 实现：关闭工程
void MainWindow::onCloseProject()
{
    if (m_currentClickedProjectItem == nullptr) return;

    // 1. 弹出确认对话框（提升用户体验，防止误操作）
    int confirm = QMessageBox::question(this, "关闭工程",
                                        QString("确定要关闭工程「%1」吗？").arg(m_currentClickedProjectItem->text()),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    // 2. 从模型中移除该工程节点（核心步骤，QT会自动递归释放该节点及其所有子节点的内存）
    // 先获取工程节点的行索引
    int row = m_currentClickedProjectItem->row();
    // 从隐形根节点中移除该行（顶层节点的父索引为QModelIndex()）
    m_projectItemModel->removeRow(row, QModelIndex());

    // 3. 关键：将当前节点指针置为nullptr，避免野指针访问（不再需要delete）
    m_currentClickedProjectItem = nullptr;

    // 4. 提示关闭成功
    QMessageBox::information(this, "关闭成功", "工程已成功关闭！");
}

// 实现：重命名工程
void MainWindow::onRenameProject()
{
    if (m_currentClickedProjectItem == nullptr) return;

    // 1. 临时开启节点可编辑状态
    m_currentClickedProjectItem->setEditable(true);

    // 2. 触发视图的编辑模式（让用户直接在视图中修改名称）
    m_projectTreeView->edit(m_currentClickedProjectItem->index());

    // 3. 优化：绑定模型的itemChanged信号，编辑完成后再恢复不可编辑状态（比QTimer更安全）
    // 先捕获当前节点的索引，用于后续判断
    QModelIndex currentIndex = m_currentClickedProjectItem->index();
    connect(m_projectItemModel, &QStandardItemModel::itemChanged, this, [=](QStandardItem *item) {
        // 只处理当前重命名的节点，避免影响其他节点
        if (item->index() == currentIndex && m_currentClickedProjectItem != nullptr) {
            item->setEditable(false);
            // 更新工程路径和tooltip
            QString newProjectName = item->text().trimmed();
            if (!newProjectName.isEmpty()) {
                QString newProjectPath = getProjectDefaultPath(newProjectName);
                item->setData(newProjectPath, Qt::UserRole + 1);
                item->setToolTip(QString("工程名称：%1\n工程路径：%2")
                                     .arg(newProjectName, newProjectPath));
            }
            // 断开信号连接，避免重复触发
            disconnect(m_projectItemModel, &QStandardItemModel::itemChanged, this, nullptr);
        }
    });
}

// 实现：保存工程
void MainWindow::onSaveProject()
{
    if (m_currentClickedProjectItem == nullptr) return;

    // 1. 模拟工程保存逻辑（后续可扩展为：解析.pro文件、持久化文件列表、保存工程配置等）
    QString projectName = m_currentClickedProjectItem->text();
    QString projectPath = m_currentClickedProjectItem->data(Qt::UserRole + 1).toString();

    // 2. 提示保存成功（展示工程信息，方便后续扩展）
    QMessageBox::information(this, "保存成功",
                             QString("工程「%1」已成功保存！\n保存路径：%2\n保存时间：%3")
                                 .arg(projectName, projectPath, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
}

// 实现：打开工程所在位置
void MainWindow::onOpenProjectLocation()
{
    if (m_currentClickedProjectItem == nullptr) return;

    // 1. 获取工程存储的路径数据
    QString projectPath = m_currentClickedProjectItem->data(Qt::UserRole + 1).toString();
    if (projectPath.isEmpty() || !QDir(projectPath).exists())
    {
        QMessageBox::warning(this, "路径错误", "工程路径不存在或无效！");
        return;
    }

    // 2. 调用系统文件管理器打开该路径（QT跨平台兼容，支持Windows/Mac/Linux）
    bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(projectPath));
    if (!success)
    {
        QMessageBox::warning(this, "打开失败", "无法打开工程所在位置，请手动检查路径！");
    }
}

// 预留：新建原理图（后续可扩展实现）
void MainWindow::onNewSchematic()
{
    if (m_currentClickedProjectItem == nullptr) return;
    QMessageBox::information(this, "预留功能", "新建原理图功能，后续可扩展实现！");
}

// 预留：新建PCB（后续可扩展实现）
void MainWindow::onNewPcb()
{
    if (m_currentClickedProjectItem == nullptr) return;
    QMessageBox::information(this, "预留功能", "新建PCB功能，后续可扩展实现！");
}

// 预留：添加已有文件（后续可扩展实现）
void MainWindow::onAddExistingFile()
{
    if (m_currentClickedProjectItem == nullptr) return;
    QMessageBox::information(this, "预留功能", "添加已有文件功能，后续可扩展实现！");
}

// 预留：关闭工程所有文件（后续可扩展实现）
void MainWindow::onCloseAllProjectFiles()
{
    if (m_currentClickedProjectItem == nullptr) return;
    QMessageBox::information(this, "预留功能", "关闭工程所有文件功能，后续可扩展实现！");
}

// 预留：工程另存为（后续可扩展实现）
void MainWindow::onSaveProjectAs()
{
    if (m_currentClickedProjectItem == nullptr) return;
    QMessageBox::information(this, "预留功能", "工程另存为功能，后续可扩展实现！");
}


// ===== 下属节点（文件/分类）功能实现（核心）=====
// 实现：保存文件
void MainWindow::onSaveFile()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 获取文件信息
    QString fileName = m_currentClickedSubItem->text();
    QString filePath = m_currentClickedSubItem->data(Qt::UserRole + 2).toString();

    // 2. 模拟文件保存逻辑
    QMessageBox::information(this, "保存成功",
                             QString("文件「%1」已成功保存！\n文件路径：%2\n保存时间：%3")
                                 .arg(fileName, filePath, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
}

// 实现：文件另存为
void MainWindow::onSaveFileAs()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 获取文件信息
    QString fileName = m_currentClickedSubItem->text();
    QString filePath = m_currentClickedSubItem->data(Qt::UserRole + 2).toString();

    // 2. 模拟另存为逻辑（后续可扩展为QFileDialog选择保存路径）
    QMessageBox::information(this, "另存为（模拟）",
                             QString("文件「%1」已模拟另存为！\n原路径：%2\n新路径：%3（后续可通过QFileDialog实现真实路径选择）")
                                 .arg(fileName, filePath, filePath + "_copy"));
}

// 实现：从工程中删除文件
void MainWindow::onRemoveFileFromProject()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 弹出确认对话框（防止误操作）
    QString itemName = m_currentClickedSubItem->text();
    int confirm = QMessageBox::question(this, "从工程删除",
                                        QString("确定要将「%1」从工程中删除吗？\n删除后仅移除工程引用，不会删除本地文件。")
                                            .arg(itemName),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    // 2. 从模型中移除下属节点（核心步骤，QT自动刷新视图，释放节点内存）
    QStandardItem *parentItem = m_currentClickedSubItem->parent();
    if (parentItem != nullptr)
    {
        // 有父节点（分类/工程），从父节点中移除
        int row = m_currentClickedSubItem->row();
        parentItem->removeRow(row);
    }
    else
    {
        // 无父节点（罕见情况，直接从模型根节点移除）
        int row = m_currentClickedSubItem->row();
        m_projectItemModel->removeRow(row, QModelIndex());
    }

    // 3. 置空指针，避免野指针
    m_currentClickedSubItem = nullptr;

    // 4. 提示删除成功
    QMessageBox::information(this, "删除成功", QString("「%1」已从工程中移除！").arg(itemName));
}

// 实现：关闭文件
void MainWindow::onCloseFile()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 模拟文件关闭逻辑（后续可扩展为关闭对应编辑器窗口）
    QString fileName = m_currentClickedSubItem->text();
    QMessageBox::information(this, "关闭成功",
                             QString("文件「%1」已成功关闭！\n（后续可扩展为关闭对应的编辑器窗口）").arg(fileName));

    // 可选：标记文件为“已关闭”状态（如修改图标、tooltip）
    m_currentClickedSubItem->setToolTip(QString("文件名称：%1\n状态：已关闭").arg(fileName));
}

// 实现：重命名文件
void MainWindow::onRenameFile()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 临时开启节点可编辑状态
    m_currentClickedSubItem->setEditable(true);

    // 2. 触发视图的编辑模式（让用户直接修改名称）
    m_projectTreeView->edit(m_currentClickedSubItem->index());

    // 3. 绑定itemChanged信号，编辑完成后恢复不可编辑，更新文件路径
    QModelIndex currentIndex = m_currentClickedSubItem->index();
    connect(m_projectItemModel, &QStandardItemModel::itemChanged, this, [=](QStandardItem *item) {
        if (item->index() == currentIndex && m_currentClickedSubItem != nullptr)
        {
            // 恢复不可编辑状态
            item->setEditable(false);

            // 2. 更新文件路径和tooltip
            QString newFileName = item->text().trimmed();
            if (!newFileName.isEmpty())
            {
                // 获取所属工程节点
                QStandardItem *projectItem = item;
                while (projectItem->parent() != nullptr)
                {
                    projectItem = projectItem->parent();
                }

                // 生成新文件路径并更新隐藏数据
                QString newFilePath = getSubItemDefaultFilePath(projectItem, item);
                item->setData(newFilePath, Qt::UserRole + 2);
                item->setToolTip(QString("文件名称：%1\n文件路径：%2\n状态：已重命名").arg(newFileName, newFilePath));
            }

            // 3. 断开信号连接，避免重复触发
            disconnect(m_projectItemModel, &QStandardItemModel::itemChanged, this, nullptr);
        }
    });
}

// 实现：打开文件所在位置
void MainWindow::onOpenFileLocation()
{
    if (m_currentClickedSubItem == nullptr) return;

    // 1. 获取下属节点存储的文件路径
    QString filePath = m_currentClickedSubItem->data(Qt::UserRole + 2).toString();
    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "路径错误", "文件路径不存在或无效！");
        return;
    }

    // 2. 区分目录和文件（分类节点是目录，文件节点是文件）
    QString targetPath = filePath;
    QFileInfo fileInfo(filePath);
    if (fileInfo.isFile())
    {
        // 打开文件所在目录（而非直接打开文件）
        targetPath = fileInfo.path();
    }

    // 3. 调用系统文件管理器打开路径（跨平台兼容）
    bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(targetPath));
    if (!success)
    {
        QMessageBox::warning(this, "打开失败", "无法打开文件所在位置，请手动检查路径！");
    }
}
