/*
    fpwin.h


    FLEXplorer, An explorer for any FLEX file or disk container
    Copyright (C) 2020-2024  W. Schwotzer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef FPWIN_INCLUDED
#define FPWIN_INCLUDED

#include "misc1.h"
#include "efiletim.h"
#include "warnoff.h"
#include <QPoint>
#include <QSize>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QList>
#include "warnon.h"
#include <functional>
#include "sfpopts.h"


class FlexplorerMdiChild;
class QAction;
class QEvent;
class QWidget;
class QToolBar;
class QLabel;
class QMenu;
class QMdiArea;
class QMimeData;
class QMdiSubWindow;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

/*------------------------------------------------------
 FLEXplorer
 The main window class
 An explorer for any FLEX file or disk container
--------------------------------------------------------*/
class FLEXplorer : public QMainWindow
{
    Q_OBJECT

public:
    FLEXplorer() = delete;
    FLEXplorer(struct sFPOptions &options);
    ~FLEXplorer() override;

    void ProcessArguments(const QStringList &args);

signals:
    void FileTimeAccessHasChanged();
    void FileSizeTypeHasChanged();

private slots:
    void NewContainer();
    void OpenContainer();
    void OpenDirectory();
    void OpenRecentDisk();
    void OpenRecentDirectory();
    void ClearAllRecentDiskEntries();
    void ClearAllRecentDirectoryEntries();
    static void Exit();
#ifndef QT_NO_CLIPBOARD
    void Copy();
    void Paste();
#endif
    void SelectAll();
    void DeselectAll();
    void FindFiles();
    void DeleteSelected();
    void InjectFiles();
    void ExtractSelected();
    void ViewSelected();
    void AttributesSelected();
    void Info();
    void Options();
    void SubWindowActivated(QMdiSubWindow *window);
    void ContextMenuRequested(QPoint pos);
    void UpdateWindowMenu();
    void CloseActiveSubWindow();
    void CloseAllSubWindows();
    void About();

public slots:
    void SelectionHasChanged();
    void SetStatusMessage(const QString &message);

private:
    void ExecuteInChild(const std::function<void(FlexplorerMdiChild &child)>&);
    QToolBar *CreateToolBar(QWidget *parent, const QString &title,
                            const QString &objectName);
    void CreateActions();
    void CreateFileActions();
    void CreateEditActions();
    void CreateContainerActions();
    void CreateExtrasActions();
    void CreateWindowsActions();
    void CreateHelpActions();
    void CreateStatusBar();
    FlexplorerMdiChild *CreateMdiChild(const QString &path,
                                       struct sFPOptions &options);
    FlexplorerMdiChild *ActiveMdiChild() const;
    QMdiSubWindow *FindMdiChild(const QString &path) const;
    void changeEvent(QEvent *event) override;
    void UpdateSelectedFiles();
    void UpdateMenus();
    static QStringList GetSupportedFiles(const QMimeData *mimeData);
    void SetFileTimeAccess(FileTimeAccess fileTimeAccess);
    void CreateRecentDiskActionsFor(QMenu *menu);
    void UpdateRecentDiskActions() const;
    void DeleteRecentDiskActions();
    void UpdateForRecentDisk(const QString &path);
    void RestoreRecentDisks();
    void CreateRecentDirectoryActionsFor(QMenu *menu);
    void UpdateRecentDirectoryActions() const;
    void DeleteRecentDirectoryActions();
    void UpdateForRecentDirectory(const QString &path);
    void RestoreRecentDirectories();
    bool OpenContainerForPath(QString path, bool isLast = true);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;


    QMdiArea *mdiArea;
    QMenu *windowMenu;
    QMenu *recentDisksMenu;
    QList<QAction *> recentDiskActions;
    QAction *recentDisksClearAllAction;
    QMenu *recentDirectoriesMenu;
    QAction *recentDirectoriesClearAllAction;
    QList<QAction *> recentDirectoryActions;
    QLabel *l_selectedFilesCount;
    QLabel *l_selectedFilesByteSize;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *containerToolBar;
    QAction *newContainerAction;
    QAction *openContainerAction;
    QAction *openDirectoryAction;
    QAction *injectAction;
    QAction *extractAction;
    QAction *selectAllAction;
    QAction *deselectAllAction;
    QAction *findFilesAction;
#ifndef QT_NO_CLIPBOARD
    QAction *copyAction;
    QAction *pasteAction;
#endif
    QAction *deleteAction;
    QAction *viewAction;
    QAction *attributesAction;
    QAction *infoAction;
    QAction *optionsAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *windowMenuSeparatorAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QSize newDialogSize;
    QSize optionsDialogSize;
    QSize attributesDialogSize;
    QString findPattern;
    QString injectDirectory;
    QString extractDirectory;
    QStringList recentDiskPaths;
    QStringList recentDirectoryPaths;
    sFPOptions &options;
};

class ProcessArgumentsFtor
{
    FLEXplorer &window;
    QStringList args;

public:
    ProcessArgumentsFtor() = delete;
    ProcessArgumentsFtor(FLEXplorer &win, int p_argc, char *p_argv[])
    : window(win)
    {
        for (int i = 0; i < p_argc; ++i)
        {
            args.push_back(p_argv[i]);
        }
    }
    ProcessArgumentsFtor(const ProcessArgumentsFtor &f)
      : window(f.window)
      , args(f.args)
    {
    }

    void operator() ()
    {
        window.ProcessArguments(args);
    }
};

#endif
