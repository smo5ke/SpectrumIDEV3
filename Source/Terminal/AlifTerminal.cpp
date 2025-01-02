#include "AlifTerminal.h"

#include <qdockwidget.h>
#include <QKeyEvent>
#include <QDebug>

Terminal::Terminal(QWidget* parent) : QDockWidget(parent), currentHistoryIndex(-1) {

    setWindowTitle("طرفية ألف");
    setStyleSheet(R"(
        QDockWidget {
            /*color: #ffffff;*/
            border: none;
        }
        QDockWidget::title {
            /*background-color: #262836;*/
            text-align: center;
            border: none;
        }
    )");

    setFeatures(QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetClosable);

    // Create terminal display
    terminalDisplay = new QTextEdit(this);
    terminalDisplay->setReadOnly(false);
    terminalDisplay->setStyleSheet(R"(
        QTextEdit {
            background-color: black;
            color: white;
            font-family: Consolas, monospace;
        }
    )");

    QWidget* dockContent = new QWidget(this);

    QVBoxLayout* vlayTerminal = new QVBoxLayout(dockContent);
    vlayTerminal->setContentsMargins(0, 0, 0, 0);
    vlayTerminal->addWidget(terminalDisplay);

    setWidget(dockContent);

    // Set initial path
    currentPath = QDir::currentPath();
    insertPrompt(true);

    // Install event filter to intercept key events
    terminalDisplay->installEventFilter(this);
}

bool Terminal::eventFilter(QObject* obj, QEvent* event) {
    if (obj == terminalDisplay && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // Handle Up and Down Arrow for command history
        if (keyEvent->key() == Qt::Key_Up) {
            navigateCommandHistory(true);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Down) {
            navigateCommandHistory(false);
            return true;
        }

        // Existing Enter key handling
        if (keyEvent->key() == Qt::Key_Return &&
            !(keyEvent->modifiers() & Qt::ControlModifier)) {

            // Get the current cursor and text
            QTextCursor cursor = terminalDisplay->textCursor();
            cursor.setPosition(commandStartPosition);
            cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
            QString command = cursor.selectedText().trimmed();

            // Store non-empty commands in history
            if (!command.isEmpty()) {
                // Avoid duplicate entries
                if (commandHistory.isEmpty() || commandHistory.last() != command) {
                    commandHistory.append(command);
                }
                // Reset history index
                currentHistoryIndex = -1;
            }

            // Handle cd command separately
            if (command.startsWith("cd ")) {
                handleCdCommand(command.mid(3).trimmed());
                return true;  // Event handled
            }

            // Execute the command
            if (!command.isEmpty()) {
                executeCommand(command);
                return true;  // Event handled
            }

            // If empty command, just insert a new prompt
            insertPrompt();
            return true;  // Event handled
        }
        // Existing backspace handling
        else if (keyEvent->key() == Qt::Key_Backspace) {
            QTextCursor cursor = terminalDisplay->textCursor();
            if (cursor.position() <= commandStartPosition) {
                return true;  // Block backspace
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void Terminal::executeCommand(const QString& command) {
    // Prepare command execution
    QString program;
    QStringList arguments;

#ifdef Q_OS_WIN
    program = "cmd.exe";
    arguments << "/c" << command;
#else
    program = "/bin/bash";
    arguments << "-c" << command;
#endif

    // Create a new QProcess for each command
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(currentPath);

    // Connect signals to handle output
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
        QByteArray output = process->readAllStandardOutput();
        terminalDisplay->moveCursor(QTextCursor::End);
        terminalDisplay->insertPlainText("\n" + QString::fromLocal8Bit(output));
        terminalDisplay->ensureCursorVisible();
        });

    connect(process, &QProcess::readyReadStandardError, this, [this, process]() {
        QByteArray error = process->readAllStandardError();
        terminalDisplay->moveCursor(QTextCursor::End);
        terminalDisplay->setTextColor(QColor("red"));
        terminalDisplay->insertPlainText("\n" + QString::fromLocal8Bit(error));
        terminalDisplay->setTextColor(QColor("white"));
        terminalDisplay->ensureCursorVisible();
        });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit) {
                terminalDisplay->moveCursor(QTextCursor::End);
                terminalDisplay->setTextColor(QColor("red"));
                terminalDisplay->insertPlainText("\nProcess crashed\n");
                terminalDisplay->setTextColor(QColor("white"));
            }
            process->deleteLater();
            insertPrompt();
        }
    );

    // Start the process
    process->start(program, arguments);
}

void Terminal::insertPrompt(bool initial) {
    // Move cursor to end
    terminalDisplay->moveCursor(QTextCursor::End);

    // If not initial call, add a newline
    if (!initial) {
        terminalDisplay->insertPlainText("\n");
    }

    // Insert path prompt
    terminalDisplay->setTextColor(QColor("lightblue"));
    terminalDisplay->insertPlainText(QString("%1> ").arg(currentPath));
    terminalDisplay->setTextColor(QColor("white"));

    // Remember the start position of command input
    commandStartPosition = terminalDisplay->textCursor().position();
}

void Terminal::handleCdCommand(const QString& path) {
    QString newPath;
    if (path.startsWith('/') || path.contains(':')) {
        // Absolute path
        newPath = path;
    }
    else {
        // Relative path
        newPath = QDir(currentPath).filePath(path);
    }

    QDir dir(newPath);
    if (dir.exists()) {
        currentPath = QDir::cleanPath(dir.absolutePath());
        QDir::setCurrent(currentPath);
        terminalDisplay->moveCursor(QTextCursor::End);
        terminalDisplay->setTextColor(QColor("green"));
        terminalDisplay->insertPlainText(QString("\nChanged directory to: %1\n").arg(currentPath));
        terminalDisplay->setTextColor(QColor("white"));
    }
    else {
        terminalDisplay->moveCursor(QTextCursor::End);
        terminalDisplay->setTextColor(QColor("red"));
        terminalDisplay->insertPlainText(QString("\nDirectory not found: %1\n").arg(newPath));
        terminalDisplay->setTextColor(QColor("white"));
    }
    insertPrompt();
}



void Terminal::navigateCommandHistory(bool previous) {
    // If no history exists, do nothing
    if (commandHistory.isEmpty()) {
        return;
    }

    // First time navigating history
    if (currentHistoryIndex == -1) {
        // Start from the last command if going previous
        currentHistoryIndex = previous ? commandHistory.size() - 1 : 0;
    }
    else {
        // Navigate through history
        if (previous) {
            // Move to previous command, but don't go below 0
            currentHistoryIndex = qMax(0, currentHistoryIndex - 1);
        }
        else {
            // Move to next command, but don't exceed history size
            currentHistoryIndex = qMin(commandHistory.size() - 1, currentHistoryIndex + 1);
        }
    }

    // Remove existing text after prompt
    QTextCursor cursor = terminalDisplay->textCursor();
    cursor.setPosition(commandStartPosition);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();

    // Insert selected history command
    terminalDisplay->insertPlainText(commandHistory[currentHistoryIndex]);
}