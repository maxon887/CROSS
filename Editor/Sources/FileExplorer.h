#ifndef FILE_EXPLORER
#define FILE_EXPLORER

#include "Cross.h"
#include "Event.h"

#include <QTreeView>

using namespace cross;

class QFileSystemModel;

class FileExplorer : public QTreeView
{
	Q_OBJECT
public:
	Event<string> FileSelected;

	FileExplorer(QWidget* parent = 0);
	~FileExplorer();

	void SetupProjectDirectory(QString dir);

protected:
	void contextMenuEvent(QContextMenuEvent *eve) override;

private:
	QFileSystemModel* file_system	= NULL;
	QMenu* context_menu				= NULL;
	QModelIndexList clipboard;

	void OnItemSelected(QModelIndex index);
	void OnItemDoubleClick(QModelIndex index);

	void OnNewFolderClick();
	void OnNewShaderClick();
	void OnNewMaterialClick();
	void OnCopyClick();
	void OnPasteClick();
	void OnDeleteClick();

	QString GetSelectedDirectory();
	QString GetAllowedName(const QString& dir, const QString& baseName, const QString& extension);
};

#endif