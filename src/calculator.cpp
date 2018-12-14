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
	sumSoFar = 0.0;
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
	Button *backspaceButton = createButton("Backspace", SLOT(backspaceClicked()));
	Button *clearButton = createButton("Clear", SLOT(clearClicked()));

	Button *addOperatorButton = createButton("+", SLOT(additiveOperatorClicked()));
	Button *minusOperatorButton = createButton("-", SLOT(additiveOperatorClicked()));

	Button *equalsButton = createButton("=", SLOT(equalsClicked()));


	//put the widgets on a grid
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout -> setSizeConstraint(QLayout::SetFixedSize);
	mainLayout -> addWidget(display,0,0,1,6);

	for(int i=1; i<NumDigitButtons; ++i){
		int row = ((9-i)/3) + 3;
		int col = (i-1)%3 + 1;
		mainLayout -> addWidget(digitButtons[i], row, col);
	}
	mainLayout -> addWidget(digitButtons[0],6,1);

	mainLayout -> addWidget(pointButton,6,2);
	mainLayout -> addWidget(changeSignButton,6,3);
	mainLayout -> addWidget(backspaceButton,2,3,1,2);
	mainLayout -> addWidget(clearButton,2,1,1,2);

	mainLayout -> addWidget(addOperatorButton,6,4);
	mainLayout -> addWidget(minusOperatorButton,5,4);

	mainLayout -> addWidget(equalsButton, 6,5);

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

void Calculator::backspaceClicked()
{
	if (waitingForOperand)
		return;

	QString text = display->text();
	text.chop(1);
	if (text.isEmpty()){
		text = "0";
		waitingForOperand = true;
	}
	display->setText(text);
}

void Calculator::clearClicked()
{
	if (waitingForOperand)
		return;

	display->setText("0");
	waitingForOperand = true;
}

void Calculator::additiveOperatorClicked()
{
	Button *clickedButton = qobject_cast<Button *>(sender());
	QString clickedOperator = clickedButton->text();
	double operand = display->text().toDouble();

	if (pendingAdditiveOperator.isEmpty()){
		sumSoFar = operand;	
	} else {
		calculate(operand, pendingAdditiveOperator);
		display->setText(QString::number(sumSoFar));
	}
	
	pendingAdditiveOperator = clickedOperator;
	waitingForOperand = true;
}

void Calculator::equalsClicked()
{
	double operand = display->text().toDouble();

	if (pendingAdditiveOperator.isEmpty()){
		sumSoFar = operand;
	} else {
		calculate(operand, pendingAdditiveOperator);
		pendingAdditiveOperator.clear();
	}

	display->setText(QString::number(sumSoFar));
	sumSoFar = 0.0;
	waitingForOperand = true;
}

Button *Calculator::createButton(const QString &text, const char *member)
{
	Button *button = new Button(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

bool Calculator::calculate(double operand, const QString &pendingOperator)
{
	if (pendingOperator == "+"){
		sumSoFar += operand;
	} else if (pendingOperator == "-"){
		sumSoFar -= operand;
	}
	return true;
}
