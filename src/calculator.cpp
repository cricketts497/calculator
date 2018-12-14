#include <QtWidgets>

#include <cmath>

#include "button.h"
#include "calculator.h"

//constructor
Calculator::Calculator(QWidget *parent) : QWidget(parent)
{
	sumInMemory = 0.0;

	//create the widgets
	display = new QLineEdit("0");
	display -> setReadOnly(true);
	display -> setAlignment(Qt::AlignRight);
	display -> setMaxLength(15);

	digitButtons = createButton(QString::number(0), SLOT(digitClicked()));

	//put the widgets on a grid
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout -> setSizeConstraint(QLayout::SetFixedSize);
	mainLayout -> addWidget(display, 0,0,1,6);

	mainLayout -> addWidget(digitButtons, 1,0);

	setLayout(mainLayout);
	setWindowTitle("Calculator");
}

void Calculator::digitClicked()
{
	Button *clickedButton = qobject_cast<Button *>(sender());
	int digitValue = clickedButton->text().toInt();


	display->setText(display->text() + QString::number(digitValue));
}

Button *Calculator::createButton(const QString &text, const char *member)
{
	Button *button = new Button(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}
