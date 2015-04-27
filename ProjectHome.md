Here i will try to dump my Qt code.

The first one is

**Frameless Window Helper**

This class can be used to give move and resize ability to any top level QWidget with Qt::FramelessWindowHint flag set.

No need to change your existing code.

Very easy to use. Just two lines of code.

```
QDialog myDialog;
NcFramelessHelper fh;
fh.activateOn(&myDialog);
myDialog.exec()
```

Any comments/bugs/suggestions are welcome.