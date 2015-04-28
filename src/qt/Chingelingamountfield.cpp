#include "Chingelingamountfield.h"
#include "qvaluecombobox.h"
#include "Chingelingunits.h"

#include "guiconstants.h"

#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QApplication>
#include <qmath.h>

ChingelingAmountField::ChingelingAmountField(QWidget *parent):
        QWidget(parent), amount(0), currentUnit(-1)
{
    amount = new QDoubleSpinBox(this);
    amount->setLocale(QLocale::c());
    amount->setDecimals(8);
    amount->installEventFilter(this);
    amount->setMaximumWidth(170);
    amount->setSingleStep(0.001);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(amount);
    unit = new QValueComboBox(this);
    unit->setModel(new ChingelingUnits(this));
    layout->addWidget(unit);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);

    setLayout(layout);

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(amount);

    // If one if the widgets changes, the combined content changes as well
    connect(amount, SIGNAL(valueChanged(QString)), this, SIGNAL(textChanged()));
    connect(unit, SIGNAL(currentIndexChanged(int)), this, SLOT(unitChanged(int)));

    // Set default based on configuration
    unitChanged(unit->currentIndex());
}

void ChingelingAmountField::setText(const QString &text)
{
    if (text.isEmpty())
        amount->clear();
    else
        amount->setValue(text.toDouble());
}

void ChingelingAmountField::clear()
{
    amount->clear();
    unit->setCurrentIndex(0);
}

bool ChingelingAmountField::validate()
{
    bool valid = true;
    if (amount->value() == 0.0)
        valid = false;
    if (valid && !ChingelingUnits::parse(currentUnit, text(), 0))
        valid = false;

    setValid(valid);

    return valid;
}

void ChingelingAmountField::setValid(bool valid)
{
    if (valid)
        amount->setStyleSheet("");
    else
        amount->setStyleSheet(STYLE_INVALID);
}

QString ChingelingAmountField::text() const
{
    if (amount->text().isEmpty())
        return QString();
    else
        return amount->text();
}

bool ChingelingAmountField::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        // Clear invalid flag on focus
        setValid(true);
    }
    else if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Comma)
        {
            // Translate a comma into a period
            QKeyEvent periodKeyEvent(event->type(), Qt::Key_Period, keyEvent->modifiers(), ".", keyEvent->isAutoRepeat(), keyEvent->count());
            qApp->sendEvent(object, &periodKeyEvent);
            return true;
        }
    }
    return QWidget::eventFilter(object, event);
}

QWidget *ChingelingAmountField::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, amount);
    return amount;
}

qint64 ChingelingAmountField::value(bool *valid_out) const
{
    qint64 val_out = 0;
    bool valid = ChingelingUnits::parse(currentUnit, text(), &val_out);
    if(valid_out)
    {
        *valid_out = valid;
    }
    return val_out;
}

void ChingelingAmountField::setValue(qint64 value)
{
    setText(ChingelingUnits::format(currentUnit, value));
}

void ChingelingAmountField::unitChanged(int idx)
{
    // Use description tooltip for current unit for the combobox
    unit->setToolTip(unit->itemData(idx, Qt::ToolTipRole).toString());

    // Determine new unit ID
    int newUnit = unit->itemData(idx, ChingelingUnits::UnitRole).toInt();

    // Parse current value and convert to new unit
    bool valid = false;
    qint64 currentValue = value(&valid);

    currentUnit = newUnit;

    // Set max length after retrieving the value, to prevent truncation
    amount->setDecimals(ChingelingUnits::decimals(currentUnit));
    amount->setMaximum(qPow(10, ChingelingUnits::amountDigits(currentUnit)) - qPow(10, -amount->decimals()));

    if(valid)
    {
        // If value was valid, re-place it in the widget with the new unit
        setValue(currentValue);
    }
    else
    {
        // If current value is invalid, just clear field
        setText("");
    }
    setValid(true);
}

void ChingelingAmountField::setDisplayUnit(int newUnit)
{
    unit->setValue(newUnit);
}
