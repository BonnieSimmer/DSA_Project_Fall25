#include "../include/GraphicalUserInterface.h"

#include <QApplication>
#include <QPlainTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>


int run_gui(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return QApplication::exec();
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    setWindowTitle("DSA Project Fall25");

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QVBoxLayout(central);

    auto *inputBrowseLayout = new QHBoxLayout();
    inputPath = new QLineEdit();
    inputPath->setPlaceholderText("Enter XML file path or browse...");
    auto *browseBtn = new QPushButton("Browse");

    inputBrowseLayout->addWidget(inputPath);
    inputBrowseLayout->addWidget(browseBtn);

    auto *displayLayout = new QHBoxLayout();

    inputText = new QTextEdit();

    outputText = new QTextEdit();
    outputText->setReadOnly(true);

    displayLayout->addWidget(inputText);
    displayLayout->addWidget(outputText);

    auto *buttonLayoutTop = new QHBoxLayout();
    auto *buttonLayoutBottom = new QHBoxLayout();
    auto *buttonLayout = new QVBoxLayout();

    auto *checkBtn = new QPushButton("Check XML");
    auto *prettyBtn = new QPushButton("Prettify XML");
    auto *convertBtn = new QPushButton("Convert to JSON");
    auto *minifiedBtn = new QPushButton("Minify XML");
    auto *compressedBtn = new QPushButton("Compress XML");
    auto *decompressedBtn = new QPushButton("Decompress XML");
    auto *clearBtn = new QPushButton("Clear Input");
    auto *saveBtn = new QPushButton("Save Output");

    buttonLayoutTop->addWidget(checkBtn);
    buttonLayoutTop->addWidget(prettyBtn);
    buttonLayoutTop->addWidget(convertBtn);
    buttonLayoutTop->addWidget(minifiedBtn);

    buttonLayoutBottom->addWidget(compressedBtn);
    buttonLayoutBottom->addWidget(decompressedBtn);
    buttonLayoutBottom->addWidget(clearBtn);
    buttonLayoutBottom->addWidget(saveBtn);

    buttonLayout->addLayout(buttonLayoutTop);
    buttonLayout->addLayout(buttonLayoutBottom);

    mainLayout->addLayout(inputBrowseLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(displayLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseClicked);
    connect(checkBtn, &QPushButton::clicked, this, &MainWindow::onCheckClicked);
    connect(prettyBtn, &QPushButton::clicked, this, &MainWindow::onPrettifyClicked);
    connect(convertBtn, &QPushButton::clicked, this, &MainWindow::onConvertClicked);
    connect(minifiedBtn, &QPushButton::clicked, this, &MainWindow::onMinifyClicked);
    connect(compressedBtn, &QPushButton::clicked, this, &MainWindow::onCompressClicked);
    connect(decompressedBtn, &QPushButton::clicked, this, &MainWindow::onDecompressClicked);
    connect(clearBtn, &QPushButton::clicked, inputText, &QTextEdit::clear);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
}

void MainWindow::onBrowseClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml)");

    if (!file.isEmpty()) {
        inputPath->setText(file);
        QString content = QString::fromStdString(FileIO::readXML(file.toStdString(), SourceType::File));
        inputText->setText(content);
    }
}

void MainWindow::onCheckClicked()
{
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }

        // TODO: add button logic here
        printOutput(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onPrettifyClicked()
{
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }
        xmlContent = XMLFormatter::format(xmlContent);
        printOutput(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onConvertClicked() {
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }
        // TODO: add button logic here
        printOutput(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onMinifyClicked()  {
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }
        xmlContent = XMLMinifier::minify(xmlContent);

        printOutput(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onCompressClicked()  {
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }

        // TODO: add button logic here
        saveOutputToFile(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onDecompressClicked()   {
    try {
        string xmlContent;
        QString userXml = inputText->toPlainText().trimmed();

        if (!userXml.isEmpty()) {
            xmlContent = FileIO::readXML(userXml.toStdString(), SourceType::GUI);
        }
        else {
            QMessageBox::warning(this, "Warning",
                "No user XML provided and no file selected.");
            return;
        }
        // TODO: add button logic here
        saveOutputToFile(xmlContent);
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::onSaveClicked()
{
    saveOutputToFile(outputText->toPlainText().toStdString());
}

void MainWindow::printOutput(const string& data) const {
    QString result = QString::fromStdString(FileIO::writeData("", data, SourceType::GUI));
    outputText->setPlainText(result);
}

void MainWindow::saveOutputToFile(const string &data) {
    QString savePath = QFileDialog::getSaveFileName(this, "Save Output", "", "XML Files (*.xml);;JSON Files (*.json)");

    if (savePath.isEmpty()) {
        return;
    }

    try {
        string status = FileIO::writeData(
            savePath.toStdString(),
            data,
            SourceType::File
        );

        QMessageBox::information(this, "Success", QString::fromStdString(status));
    }
    catch (const exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}
