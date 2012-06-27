#ifndef SELECTFILE_H
#define SELECTFILE_H

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

	signals:
		void fileSelected(QString rightFile, QString leftFile);
		
	public slots:
		void rightSaveAs();
		void leftSaveAs();
		void validate();

	public:
		SelectFile(QWidget *parent=0);
		~SelectFile(void);
};

#endif // SELECTFILE_H