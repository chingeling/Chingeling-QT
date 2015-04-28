#include "form.h"
#include "ui_form.h"
#include <QWidget>
#include <QStackedWidget>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
  /*  m_pWebView = new QWebView(this);


        m_pWebView->load(QUrl("http://www.google.com"));
      m_pWebView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
*/

}

Form::~Form()
{
    delete ui;


  //  ui->setupUi(this);
  //  this->webView->load(QUrl("http://chingeling.com/ecommerce"));

}

