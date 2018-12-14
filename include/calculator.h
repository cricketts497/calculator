#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

class QLineEdit;

class Button;

class Calculator : public QWidget
{
	Q_OBJECT

public:
	Calculator(QWidget *parent = 0);

private slots:
	void digitClicked();

private:
	Button *createButton(const QString &text, const char *member);

	double sumInMemory;
	bool waitingForOperand;

	QLineEdit *display;

	enum { NumDigitButtons = 10 };
	Button *digitButtons[NumDigitButtons];
};


#endif //CALCULATOR_H
