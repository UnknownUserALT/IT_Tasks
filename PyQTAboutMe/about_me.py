import sys
from PySide6.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QVBoxLayout


class AboutMeWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Обо мне")
        self.setFixedSize(300, 150)

        # Создаем виджеты
        self.label = QLabel("Титова Алина\nСтудент, программист")
        self.button = QPushButton("Показать больше")

        # Компоновка
        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(self.button)
        self.setLayout(layout)

        # Подключаем кнопку
        self.button.clicked.connect(self.change_text)
        self.expanded = False

    def change_text(self):
        if not self.expanded:
            self.label.setText("Титова Алина\nСтудент, программист\nЛюблю Python и Qt!")
            self.button.setText("Скрыть")
        else:
            self.label.setText("Алина Титова\nСтудент, программист")
            self.button.setText("Показать больше")
        self.expanded = not self.expanded


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = AboutMeWindow()
    window.show()
    sys.exit(app.exec_())
