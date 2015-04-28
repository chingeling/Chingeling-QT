#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QWebView>
#include <QUrl>
#include <QStackedWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;
    QWebView* m_pWebView;
      QStackedWidget *centralWidget;
};

#endif // FORM_H
