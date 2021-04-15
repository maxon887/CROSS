#include "FileHandler.h"
#include "FileExplorer.h"
#include "CrossEditor.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QFileInfo>

FileHandler::FileHandler(QWidget* parent)
	: QLineEdit(parent)
{ }

QString FileHandler::GetFile() {
	return file_path;
}

void FileHandler::SetFile(const QString& filename) {
	file_path = filename;
	QFileInfo info = QFileInfo(filename);
	setText(info.fileName());
	FileChanged.Emit(filename);
}

void FileHandler::SetFileExtension(const QString& ext) {
	file_extension = ext;
}

void FileHandler::dragEnterEvent(QDragEnterEvent *event) {
	drop_accepted = false;
	const QMimeData* data = event->mimeData();
	QList<QUrl> urls = data->urls();
	if(file_extension == "") {
		event->ignore();

		QMessageBox* msgBox = new QMessageBox(this);
		msgBox->setText("Something goes wrong");
		msgBox->setInformativeText("File extension not specified for this handle");
		msgBox->setIcon(QMessageBox::Icon::Warning);
		msgBox->show();
		return;
	}
	if(urls.size() != 1) {
		event->ignore();

		QMessageBox* msgBox = new QMessageBox(this);
		msgBox->setText("Something goes wrong");
		msgBox->setInformativeText("File Handler can contain only one file");
		msgBox->setIcon(QMessageBox::Icon::Warning);
		msgBox->show();
		return;
	}
	QString filename = urls.at(0).fileName();
	if(filename.endsWith("." + file_extension)) {
		drop_accepted = true;
		event->accept();
	}
}

void FileHandler::dragMoveEvent(QDragMoveEvent *event) {
	if(drop_accepted) {
		event->accept();
	}
}

void FileHandler::dropEvent(QDropEvent *event) {
	const QMimeData* data = event->mimeData();
	QList<QUrl> urls = data->urls();
	QString filename = urls.at(0).toLocalFile();
	filename = editor->GetFileExplorer()->GetRelativePath(filename);
	SetFile(filename);
}