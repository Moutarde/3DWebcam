#ifndef SELECTFILE_H
#define SELECTFILE_H

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <QFileDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QWidget>

using namespace std;

class SelectFile : public QWidget {
	Q_OBJECT

	private:
		QLineEdit *rightLineEdit;
		QLineEdit *leftLineEdit;
		int nbCam;

	signals:
		void fileSelected(QString rightFile, QString leftFile);
		void fileSelected(QString file);
		
	public slots:
		void rightSaveAs();
		void leftSaveAs();
		void validate();

	public:
		SelectFile(int nbCam = 2, QWidget *parent = 0);
		~SelectFile(void);
};

#endif // SELECTFILE_H