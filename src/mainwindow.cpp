#include "mainwindow.h"
#include <QCompleter>
#include <QSettings>
#include <QDir>

#if QT_VERSION >= 0x050000
    #include <QCryptographicHash>
    #define CryptographicHash QCryptographicHash
#else
    #include "qt5cryptographichash.h"
    #define CryptographicHash Qt5CryptographicHash
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    ui.algorithm->setCurrentIndex(CryptographicHash::Sha3_512);

    setCompleter(ui.domain, "domain");
    setCompleter(ui.desc, "desc");
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_generateButton_clicked()
{
    CryptographicHash::Algorithm hashAlgorithm = static_cast<CryptographicHash::Algorithm>(ui.algorithm->currentIndex());

    CryptographicHash hash(hashAlgorithm);
    QString domain = ui.domain->text().simplified();
    QString desc = ui.desc->text().simplified();

    ui.domain->setText(domain);
    ui.desc->setText(desc);

    QByteArray chash = hash.hash(ui.master->text().toUtf8() +
                                 domain.toUtf8() +
                                 desc.toUtf8(),
                                 hashAlgorithm);

    QString base64Hash = chash.toBase64();
    ui.pwd->setText(base64Hash.left(ui.pwdLen->value()));

    saveWordList("domain", domain);
    setCompleter(ui.domain, "domain");
    saveWordList("desc", desc);
    setCompleter(ui.desc, "desc");
}

void MainWindow::on_showPw_toggled(bool checked)
{
    if (checked)
    {
        ui.master->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui.master->setEchoMode(QLineEdit::Password);
    }
}

void MainWindow::on_copyButton_clicked()
{
    ui.pwd->selectAll();
    ui.pwd->copy();
}

void MainWindow::setCompleter(QLineEdit *lineEdit, const QString &listName)
{
    QCompleter *completer = lineEdit->completer();

    if (completer)
        delete completer;

    completer = new QCompleter(loadWordList(listName), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    lineEdit->setCompleter(completer);
}

QStringList MainWindow::loadWordList(const QString &listName)
{
    QStringList words;
    QString path = configPath();

    QSettings settings(path, QSettings::IniFormat);
    int size = settings.beginReadArray(listName);
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        words.append(settings.value("item").toString());
    }
    settings.endArray();

    return words;
}

QStringList MainWindow::saveWordList(const QString &listName, QString newWord)
{
    QStringList words = loadWordList(listName);

    words.append(newWord);
    words.removeDuplicates();

    QString path = configPath();
    QDir dir(path);
    dir.mkpath(dir.absolutePath());

    QSettings settings(path, QSettings::IniFormat);
    settings.beginWriteArray(listName);
    for (int i = 0; i < words.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("item", words.at(i));
    }
    settings.endArray();
    settings.sync();

    return words;
}

QString MainWindow::configPath()
{
    QString path;

    #ifdef Q_OS_ANDROID
        path = "/sdcard/passwd-gen.ini";
    #elif defined(Q_WS_MAEMO_5)
        path = "/home/user/.passwd-gen/passwd-gen.conf";
    #else
        path = "passwd-gen.ini";
    #endif

    return path;
}
