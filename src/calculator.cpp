#include <QtWidgets>
#include <iostream>
#include <cstring>

#include <cmath>

#include "include/button.h"
#include "include/calculator.h"

//constructor
Calculator::Calculator(QWidget *parent) : QWidget(parent)
{
	sumInMemory = 0.0;
	waitingForOperand = true;

	//create the widgets
	display = new QLineEdit("0");
	display -> setReadOnly(true);
	display -> setAlignment(Qt::AlignRight);
	display -> setMaxLength(15);

	for(int i=0; i<NumDigitButtons; ++i){
		digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
	}

	Button *pointButton = createButton(".", SLOT(pointClicked()));
	Button *changeSignButton = createButton("\302\261", SLOT(changeSignClicked()));


	//put the widgets on a grid
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout -> setSizeConstraint(QLayout::SetFixedSize);
	mainLayout -> addWidget(display, 0,0,1,6);

	for(int i=1; i<NumDigitButtons; ++i){
		int row = ((9-i)/3) + 2;
		int col = (i-1)%3 + 1;
		mainLayout -> addWidget(digitButtons[i], row, col);
	}
	mainLayout -> addWidget(digitButtons[0],5,1);

	mainLayout -> addWidget(pointButton,5,2);
	mainLayout -> addWidget(changeSignButton,5,3);

	setLayout(mainLayout);
	setWindowTitle("Calculator");
}

void Calculator::digitClicked()
{
	Button *clickedButton = qobject_cast<Button *>(sender());
	int digitValue = clickedButton->text().toInt();

	//don't add extra zeros
	if (display->text() == "0" && digitValue == 0.0){
		return;
	}

	if (waitingForOperand){
		display->clear();
		waitingForOperand = false;
	}

	display->setText(display->text() + QString::number(digitValue));
}

void Calculator::pointClicked()
{
	if (waitingForOperand)
		display->setText("0");
	if (!display->text().contains('.'))
		display->setText(display->text() + ".");
	waitingForOperand = false;
}

void Calculator::changeSignClicked()
{
	QString text = display->text();
	double value = text.toDouble();

	if (value > 0.0){
		text.prepend('-');
	}else if (value < 0.0){
		text.remove(0,1);
	}

	display->setText(text);
}

Button *Calculator::createButton(const QString &text, const char *member)
{
	Button *button = new Button(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}
