#include "descriptioninfo.h"

DescriptionInfo::DescriptionInfo() {
  this->m_description = "";
  this->m_name = "";
  this->m_url = "";
}

DescriptionInfo::DescriptionInfo(QString name, QString description,
                                 QString url) {
  this->m_name = name;
  this->m_description = description;
  this->m_url = url;
}

QString DescriptionInfo::name() const { return this->m_name; }
QString DescriptionInfo::description() const { return this->m_description; }
QString DescriptionInfo::url() const { return this->m_url; }
