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
#include <QTextBlock>


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
      "* {"
        "   font-size: 12pt;"
        "   font-family: Arial;"
        "}"
        "QPushButton {"
        "   background-color: #0d6efd;"
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1454b3;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1a4078;"
        "}"
    );

    connect(inputText, &QTextEdit::textChanged, this, [this]() {
    isDataDirty = true;
    inputText->setExtraSelections({});
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

void MainWindow::onCheckClicked() {
    QString currentContent = inputText->toPlainText();
    inputText->setExtraSelections({}); // Clear old highlights

    if (currentContent.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Input area is empty.");
        return;
    }

    string report;
    vector<XMLError> errors = ParseError::verify(currentContent.toStdString(), false, report);

    if (errors.empty()) {
        printOutput("Success: XML is valid.");
        lastValidationResult = true;
        isDataDirty = false;
    } else {
        printOutput(report);
        QList<QTextEdit::ExtraSelection> allSelections;

        for (const XMLError& error : errors) {
            QTextEdit::ExtraSelection selection;
            QTextBlock block = inputText->document()->findBlockByLineNumber(error.line - 1);

            QTextCursor cursor(block);
            cursor.select(QTextCursor::LineUnderCursor);

            selection.format.setBackground(Qt::red);
            selection.format.setForeground(Qt::white);
            selection.cursor = cursor;

            allSelections.append(selection);
        }

        inputText->setExtraSelections(allSelections);

        if (!errors.empty()) {
            QTextBlock firstBlock = inputText->document()->findBlockByLineNumber(errors[0].line - 1);
            inputText->setTextCursor(QTextCursor(firstBlock));
        }

        auto reply = QMessageBox::question(this, "XML Errors Found",
                                          "Multiple errors detected. Auto-fix them?",
                                          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            string fixedXml = ParseError::solveErrors(currentContent.toStdString(), errors);
            inputText->setText(QString::fromStdString(XMLFormatter::format(fixedXml)));
            printOutput("XML fixed and updated.");
            isDataDirty = true;
        }
        lastValidationResult = false;
    }
}

void MainWindow::highlightErrorLine(int lineNumber) {
    QTextDocument *doc = inputText->document();
    QTextBlock block = doc->findBlockByLineNumber(lineNumber - 1); // 0-indexed

    QTextCursor cursor(block);
    cursor.select(QTextCursor::LineUnderCursor);

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(Qt::red);
    selection.format.setForeground(Qt::white);
    selection.cursor = cursor;

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    inputText->setExtraSelections(extraSelections);

    inputText->setTextCursor(cursor);
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
        type = Type::XML;
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
        type = Type::XML;
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
            reportInfo("Success","Decompressed Successfully");
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
        reportInfo("Success",status);
    }
    catch (const exception &ex) {
        reportError("Error", ex.what());
    }
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
        idInput->setFocus();
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
        idInput->clear();
        idInput->setFocus();
        return;
    }
    printOutput(networkAnalyzer->suggestUsersToFollow(idInput->text().toInt()));
}

void MainWindow::onSearchClicked() {
    auto* engine = getReadySearchEngine();
    if (!engine) return;

    std::string query = searchInput->text().toStdString();
    if (query.empty()) {
        QMessageBox::warning(this, "Warning", "Please enter a search term.");
        searchInput->setFocus();
        return;
    }

    std::vector<Post> results;
    if (wordRadio->isChecked()) {
        results = engine->searchByWord(query);
    } else {
        results = engine->searchByTopic(query);
    }

    if (results.empty()) {
        printOutput("No posts found for: " + query);
    } else {
        std::stringstream ss;
        ss << "Found " << results.size() << " post(s):\n";
        ss << "----------------------\n";
        for (const auto& post : results) {
            ss << "Body: " << post.getBody() << "\n";
            ss << "Topics: ";
            for (const auto& t : post.getTopics()) ss << "[" << t << "] ";
            ss << "\n----------------------\n";
        }
        printOutput(ss.str());
    }
}

void MainWindow::onDrawGraphClicked() {
    string xmlContent = inputText->toPlainText().toStdString();

    XMLParser parser;
    parser.parse(xmlContent);

    QString savePath = QFileDialog::getSaveFileName(this, "Save Graph", "graph.jpg", "Images (*.jpg)");

    if (!savePath.isEmpty()) {
        GraphVisualizer::draw(parser.users, savePath.toStdString());
    }
}

bool MainWindow::validateAndFixXML(QString& content) {
    if (!isDataDirty && lastValidationResult) {
        return true;
    }

    string report;
    vector<XMLError> errors = ParseError::verify(content.toStdString(), false, report);

    if (errors.empty()) {
        lastValidationResult = true;
        isDataDirty = false;
        return true;
    }

    QString prompt = QString("Errors found (e.g., %1). Analysis requires valid XML.\n\nAuto-fix now?")
                     .arg(QString::fromStdString(report).split('\n').first());

    auto reply = QMessageBox::question(this, "Validation Required", prompt,
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        string fixedXML = ParseError::solveErrors(content.toStdString(), errors);
        content = QString::fromStdString(fixedXML);
        inputText->setText(content);
        isDataDirty = false;
        lastValidationResult = true;
        return true;
    }

    lastValidationResult = false;
    return false;
}

NetworkAnalyzer* MainWindow::getReadyAnalyzer() {
    QString currentContent = inputText->toPlainText().trimmed();

    if (!isDataDirty && analyzer != nullptr) {
        return analyzer.get();
    }

    if (currentContent.isEmpty()) {
        reportError("Empty Input", "Please provide XML data first.");
        inputText->setFocus();
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

PostSearch* MainWindow::getReadySearchEngine() {
    QString currentContent = inputText->toPlainText().trimmed();

    if (!isDataDirty && searchEngine != nullptr) {
        return searchEngine.get();
    }

    if (currentContent.isEmpty()) {
        reportError("Empty Input", "Please provide XML data first.");
        inputText->setFocus();
        return nullptr;
    }

    XMLParser parser;
    parser.parse(currentContent.toStdString());
    searchEngine = std::make_unique<PostSearch>(parser.users);

    isDataDirty = false;
    return searchEngine.get();
}
