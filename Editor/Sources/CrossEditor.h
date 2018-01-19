#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"
#include "ui_CrossEditor.h"

#include <QMainWindow>

class Action;

class CrossEditor : public QMainWindow, 
					public Game
{
	Q_OBJECT
public:
	Event<> UIInitialized;
	Event<Action*> SomethingChanged;

	CrossEditor(QWidget *parent = 0);

	void Update(float sec) override;

	SceneExplorer* GetSceneExplorer();
	PropertiesView* GetPropertyView();
	FileExplorer* GetFileExplorer();

	void LoadScene(QString& fileInfo);
	void RestoreSettings();
	void ExceptionMsgBox(const char* msg);
	void AdjustSize(QWidget* widget);

	void OnNewSceneClick();
	void OnSaveAsClick();
	void OnSetupProjectDirectoryClick();

	void OnScreenChanged(Screen* screen);
	void OnSomethingChanged(Action* a);

protected:
	void closeEvent(QCloseEvent* eve) override;
	void keyPressEvent(QKeyEvent* key) override;
	void keyReleaseEvent(QKeyEvent* key) override;

private:
	Ui::CrossEditorClass ui;
	QString scene_file;

};

extern CrossEditor* editor;

#endif // !CROSS_EDITOR