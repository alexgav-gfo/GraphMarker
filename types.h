#ifndef TYPES_H
#define TYPES_H

#include <QRegExp>

const QRegExp FILENAME_PATTERN_DAYOFYEAR("^[0-9]{4}_[0-9]{3}_[a-z]{2}_[a-z]{2}_[a-z]{2}.png$");
const QRegExp FILENAME_PATTERN_DAYABS("^[0-9]{4}_[0-9]{4}_[a-z]{2}_[a-z]{2}_[a-z]{2}.png$");
const QRegExp FILENAME_PATTERN_DAYABS_NUM("^[0-9]{4}_[0-9]{4}_[a-z]{2}_[a-z]{2}_[a-z]{2}_[0-9].png$");

const QRegExp INI_PATTERN_DAYOFYEAR("^[0-9]{4}_[0-9]{3}_[a-z]{2}_[a-z]{2}_[a-z]{2}.ini$");
const QRegExp INI_PATTERN_DAYABS("^[0-9]{4}_[0-9]{4}_[a-z]{2}_[a-z]{2}_[a-z]{2}.ini$");
const QRegExp INI_PATTERN_DAYABS_NUM("^[0-9]{4}_[0-9]{4}_[a-z]{2}_[a-z]{2}_[a-z]{2}_[0-9].ini$");

#endif // TYPES_H
