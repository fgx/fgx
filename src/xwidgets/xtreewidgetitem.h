
#include <QtGui/QTreeWidgetItem>

class XTreeWidgetItem : public QTreeWidgetItem
{
public:
XTreeWidgetItem( QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
XTreeWidgetItem( QTreeWidgetItem *item) : QTreeWidgetItem(item)  {}
XTreeWidgetItem( QTreeWidget * parent, const QStringList & strings) : QTreeWidgetItem (parent,strings)  {}

bool operator< (const QTreeWidgetItem &other) const
{
	int sortCol = treeWidget()->sortColumn();
	return text(sortCol).toUpper() < other.text(sortCol).toUpper();
}
};
