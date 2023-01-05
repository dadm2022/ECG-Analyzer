//
// Created by Piotr on 23.12.2022.
//

#include "TableUtils.h"

namespace TableUtils {
    QTableWidgetItem *createCenterAllignedTableItem(const QString &text) {
        auto *result = new QTableWidgetItem(text);
        result->setTextAlignment(Qt::AlignCenter);
        return result;
    }
}