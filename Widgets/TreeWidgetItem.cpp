#include <QDateTime>

#include "TreeWidgetItem.h"
#include "Constants.h"

const QLocale TreeWidgetItem::locale = QLocale::English;

TreeWidgetItem::TreeWidgetItem(QTreeWidgetItem *pItem) noexcept : QTreeWidgetItem(pItem)
{}

bool TreeWidgetItem::operator<(const QTreeWidgetItem &other) const noexcept
{
    switch (int column = treeWidget()->sortColumn())
    {
        case 1:
            return this->text(column).toUInt() < other.text(column).toUInt();
        case 2:
        {
            const auto data = this->text(column);
            const auto otherData = other.text(column);

            const auto date = data.indexOf(":") != -1 ? locale.toDateTime(data, Constants::TIME_FORMAT).addYears(
                    QDate::currentDate().year()) : locale.toDateTime(data, Constants::YEAR_FORMAT);

            const auto otherDate =
                    otherData.indexOf(":") != -1 ? locale.toDateTime(otherData, Constants::TIME_FORMAT).addYears(
                            QDate::currentDate().year()) : locale.toDateTime(otherData, Constants::YEAR_FORMAT);

            return date < otherDate;
        }
        default:
            return QTreeWidgetItem::operator<(other);
    }
}
