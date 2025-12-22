#include "../include/GraphicalUserInterface.h"

#include <QApplication>
#include <QPlainTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>


int run_gui(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return QApplication::exec();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DSA Project");
    setMinimumSize(1000, 800);

    auto *central = new QWidget(this);
    setCentralWidget(central);
    auto *mainLayout = new QVBoxLayout(central);

    inputPath = new QLineEdit();
    inputText = new QTextEdit();
    outputText = new QTextEdit();

    inputPath->setPlaceholderText("Path to XML or compressed file");
    inputPath->setReadOnly(true);
    inputText->setPlaceholderText("Input XML content...");
    outputText->setReadOnly(true);
    outputText->setPlaceholderText("Output results...");
    inputText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *inputBrowseLayout = new QHBoxLayout();
    auto *browseBtn = new QPushButton("Browse");
    inputBrowseLayout->addWidget(inputPath);
    inputBrowseLayout->addWidget(browseBtn);
    mainLayout->addLayout(inputBrowseLayout);

    QTabWidget *tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QWidget *tab1 = new QWidget();
    auto *layout1 = new QVBoxLayout(tab1);
    setupLevel1Layout(layout1);
    tabs->addTab(tab1, "XML Utilities (Lvl 1)");

    QWidget *tab2 = new QWidget();
    auto *layout2 = new QVBoxLayout(tab2);
    setupLevel2Layout(layout2);
    tabs->addTab(tab2, "Network Analysis (Lvl 2)");

    mainLayout->addWidget(tabs);

    auto *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(inputText);
    displayLayout->addWidget(outputText);
    mainLayout->addLayout(displayLayout, 1);

    this->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1f6391;"
        "}"
    );

    connect(inputText, &QTextEdit::textChanged, this, [this]() {
    isDataDirty = true;
    });
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseClicked);
}

void MainWindow::setupLevel1Layout(QVBoxLayout *layout) {
    auto *btnGrid = new QGridLayout();

    auto *checkBtn = new QPushButton("Check XML");
    auto *prettyBtn = new QPushButton("Prettify");
    auto *minifyBtn = new QPushButton("Minify");
    auto *convertBtn = new QPushButton("To JSON");
    auto *compressBtn = new QPushButton("Compress");
    auto *decompressBtn = new QPushButton("Decompress");
    auto *clearBtn = new QPushButton("Clear");
    auto *saveBtn = new QPushButton("Save Output");

    btnGrid->addWidget(checkBtn, 0, 0);
    btnGrid->addWidget(prettyBtn, 0, 1);
    btnGrid->addWidget(minifyBtn, 0, 2);
    btnGrid->addWidget(convertBtn, 1, 0);
    btnGrid->addWidget(compressBtn, 1, 1);
    btnGrid->addWidget(decompressBtn, 1, 2);
    btnGrid->addWidget(clearBtn, 2, 0, 1, 3);
    btnGrid->addWidget(saveBtn, 3, 0, 1, 3);

    layout->addLayout(btnGrid);

    layout->addStretch(1);

    connect(checkBtn, &QPushButton::clicked, this, &MainWindow::onCheckClicked);
    connect(prettyBtn, &QPushButton::clicked, this, &MainWindow::onPrettifyClicked);
    connect(minifyBtn, &QPushButton::clicked, this, &MainWindow::onMinifyClicked);
    connect(convertBtn, &QPushButton::clicked, this, &MainWindow::onConvertClicked);
    connect(compressBtn, &QPushButton::clicked, this, &MainWindow::onCompressClicked);
    connect(decompressBtn, &QPushButton::clicked, this, &MainWindow::onDecompressClicked);
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        inputText->clear();
        inputPath->clear();
        outputText->clear();
        type = Type::OTHER;
    });
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
}

void MainWindow::setupLevel2Layout(QVBoxLayout *layout) {
    layout->setAlignment(Qt::AlignTop);

    QGroupBox *globalBox = new QGroupBox("Global Statistics");
    QHBoxLayout *globalLayout = new QHBoxLayout(globalBox);
    auto *influencerBtn = new QPushButton("Most Influencer");
    auto *activeBtn = new QPushButton("Most Active");
    auto *drawBtn = new QPushButton("Draw Graph (JPG)");
    globalLayout->addWidget(influencerBtn);
    globalLayout->addWidget(activeBtn);
    globalLayout->addWidget(drawBtn);

    QGroupBox *userBox = new QGroupBox("User Analysis");
    QGridLayout *userLayout = new QGridLayout(userBox);
    idInput = new QLineEdit();
    idInput->setPlaceholderText("e.g. 1,2,3");
    auto *suggestBtn = new QPushButton("Suggest Users");
    auto *mutualBtn = new QPushButton("Find Mutual");
    userLayout->addWidget(new QLabel("Target ID(s):"), 0, 0);
    userLayout->addWidget(idInput, 0, 1);
    userLayout->addWidget(suggestBtn, 1, 0);
    userLayout->addWidget(mutualBtn, 1, 1);

    QGroupBox *searchBox = new QGroupBox("Post Search");
    QHBoxLayout *searchLayout = new QHBoxLayout(searchBox);
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search...");
    wordRadio = new QRadioButton("Word");
    topicRadio = new QRadioButton("Topic");
    wordRadio->setChecked(true);
    auto *searchBtn = new QPushButton("Search Posts");
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(wordRadio);
    searchLayout->addWidget(topicRadio);
    searchLayout->addWidget(searchBtn);

    layout->addWidget(globalBox);
    layout->addWidget(userBox);
    layout->addWidget(searchBox);

    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(2);

    connect(influencerBtn, &QPushButton::clicked, this, &MainWindow::onMostInfluencerClicked);
    connect(activeBtn, &QPushButton::clicked, this, &MainWindow::onMostActiveClicked);
    connect(suggestBtn, &QPushButton::clicked, this, &MainWindow::onSuggestClicked);
    connect(mutualBtn, &QPushButton::clicked, this, &MainWindow::onMutualClicked);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(drawBtn, &QPushButton::clicked, this, &MainWindow::onDrawGraphClicked);
}

void MainWindow::onBrowseClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;Compressed Files (*.comp)");

    if (!file.isEmpty()) {
        inputPath->setText(file);
        if (file.endsWith(".xml")) {
            type = Type::XML;
            QString content = QString::fromStdString(FileIO::readXML(file.toStdString(), SourceType::File));
            inputText->setText(content);
            return;
        }
        type = Type::OTHER;
        inputText->clear();
    }
}

void MainWindow::onCheckClicked()
{
    try {
        if (type == Type::OTHER) {
            QMessageBox::warning(this, "Warning",
                            "No XML file is selected.");
            return;
        }
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
        reportError("Error", ex.what());
    }
}

void MainWindow::onPrettifyClicked()
{
    try {
        if (type == Type::OTHER) {
            QMessageBox::warning(this, "Warning",
                            "No XML file is selected.");
            return;
        }
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
        reportError("Error", ex.what());
    }
}

void MainWindow::onConvertClicked() {
    try {
        if (type == Type::OTHER) {
            QMessageBox::warning(this, "Warning",
                            "No XML file is selected.");
            return;
        }
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
        xmlContent = convertXMLToJSON(xmlContent);
        type = Type::JSON;

        printOutput(xmlContent);
    }
    catch (const exception &ex) {
        reportError("Error", ex.what());
    }
}

void MainWindow::onMinifyClicked()  {
    try {
        if (type == Type::OTHER) {
            QMessageBox::warning(this, "Warning",
                            "No XML file is selected.");
            return;
        }
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
        reportError("Error", ex.what());
    }
}

void MainWindow::onCompressClicked()  {
    try {
        if (type == Type::OTHER) {
            QMessageBox::warning(this, "Warning",
                            "No XML file is selected.");
            return;
        }
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
        saveOutputToFile(xmlContent,true);
    }
    catch (const exception &ex) {
        reportError("Error", ex.what());
    }
}

void MainWindow::onDecompressClicked()   {
    try {
        if (type == Type::OTHER) {
            QString savePath = QFileDialog::getSaveFileName(this, "Save Output", "", "XML Files (*.xml)");
            if (savePath.isEmpty()) {
                return;
            }
            decompressor.decompress(inputPath->text().toStdString(), savePath.toStdString());
            string xmlContent = FileIO::readXML(savePath.toStdString(), SourceType::File);
            printOutput(xmlContent);
            QMessageBox::information(this, "Success", "Decompressed Successfully");

        } else {
            QMessageBox::warning(this, "Warning",
                            "No Compressed XML file is selected.");
        }
    }
    catch (const exception &ex) {
        reportError("Error", ex.what());
    }
}

void MainWindow::onSaveClicked()
{
    string outputType = type == Type::XML ? "XML Files (*.xml)" : "JSON Files (*.json)";
    saveOutputToFile(outputText->toPlainText().toStdString(),false ,outputType);
}

void MainWindow::printOutput(const string& data) const {
    QString result = QString::fromStdString(FileIO::writeData("", data, SourceType::GUI));
    outputText->setPlainText(result);
}

void MainWindow::saveOutputToFile(const string &data, bool compress, const string &outputFileType) {
    QString savePath = QFileDialog::getSaveFileName(this, "Save Output", "", outputFileType.data());

    if (savePath.isEmpty()) {
        return;
    }
    try {
        string status;
        if (compress) {
            compressor.compress(data,savePath.toStdString());
            status = "Compressed Successfully";
        } else {
            status = FileIO::writeData(
           savePath.toStdString(),
           data,
           SourceType::File
           );
        }

        QMessageBox::information(this, "Success", QString::fromStdString(status));
    }
    catch (const exception &ex) {
        reportError("Error", ex.what());
    }
}

NetworkAnalyzer* MainWindow::getReadyAnalyzer() {
    QString currentContent = inputText->toPlainText().trimmed();

    if (!isDataDirty && analyzer != nullptr) {
        return analyzer.get();
    }

    if (currentContent.isEmpty()) {
        reportError("Empty Input", "Please provide XML data first.");
        return nullptr;
    }

    if (!validateAndFixXML(currentContent)) {
        return nullptr;
    }

    XMLParser parser;
    parser.parse(currentContent.toStdString());
    analyzer = std::make_unique<NetworkAnalyzer>(parser.users);

    isDataDirty = false;
    return analyzer.get();
}

void MainWindow::onMostInfluencerClicked() {
    auto* networkAnalyzer = getReadyAnalyzer();
    if (!networkAnalyzer) return;

    printOutput(networkAnalyzer->MostInfluencerUser());
}

void MainWindow::onMostActiveClicked() {
    auto* networkAnalyzer = getReadyAnalyzer();
    if (!networkAnalyzer) return;

    printOutput(networkAnalyzer->MostActiveUser());
}

void MainWindow::onMutualClicked() {
    auto* networkAnalyzer = getReadyAnalyzer();
    if (!networkAnalyzer) return;

    if (idInput->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide target IDs.");
        return;
    }
    vector<int> targetIds = FileIO::parseIds(idInput->text().toStdString());

    printOutput(networkAnalyzer->mutualFollowers(targetIds));
}

void MainWindow::onSuggestClicked() {
    auto* networkAnalyzer = getReadyAnalyzer();
    if (!networkAnalyzer) return;

    if (idInput->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide target IDs.");
        idInput->clear();
        idInput->setFocus();
        return;
    }
    if (idInput->text().contains(",")) {
        QMessageBox::warning(this, "Warning", "Please provide one ID per line (no commas).");
        return;
    }
    printOutput(networkAnalyzer->suggestUsersToFollow(idInput->text().toInt()));
}

void MainWindow::onSearchClicked() {

}

void MainWindow::onDrawGraphClicked() {
    string xmlContent = inputText->toPlainText().toStdString();

    XMLParser parser;
    parser.parse(xmlContent);

    QString savePath = QFileDialog::getSaveFileName(this, "Save Graph", "graph.jpg", "Images (*.jpg)");

    if (!savePath.isEmpty()) {
        GraphVisualizer::draw(parser.users, savePath.toStdString());
        QMessageBox::information(this, "Success", "Graph saved to " + savePath);
    }
}

bool MainWindow::validateAndFixXML(QString& content) {
    return true;
}
