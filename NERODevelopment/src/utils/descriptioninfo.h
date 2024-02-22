#ifndef DESCRIPTIONINFO_H
#define DESCRIPTIONINFO_H

#include <QString>

class DescriptionInfo {
public:
  DescriptionInfo();
  explicit DescriptionInfo(QString, QString, QString);

  QString name() const;
  QString description() const;
  QString url() const;

private:
  QString m_name;
  QString m_description;
  QString m_url;
};

#endif // DESCRIPTIONINFO_H
