#ifndef ChingelingADDRESSVALIDATOR_H
#define ChingelingADDRESSVALIDATOR_H

#include <QRegExpValidator>

/** Base48 entry widget validator.
   Corrects near-miss characters and refuses characters that are no part of base48.
 */
class ChingelingAddressValidator : public QValidator
{
    Q_OBJECT
public:
    explicit ChingelingAddressValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;

    static const int MaxAddressLength = 35;
signals:

public slots:

};

#endif // ChingelingADDRESSVALIDATOR_H
