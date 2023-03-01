#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <stdio.h>
#include <QFile>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QRect>
#include <QScreen>
#include <QDesktopServices>
#include <QChar>
#include <QPalette>
#include <QTextStream>

class Destinatie{
private:
    int numarCurs;
    int numarPagina;
    int numarParagraf;
    QString link;

public:
    //incapsulare
    int getNumarCurs() { return numarCurs; }
    int getNumarPagina() { return numarPagina; }
    int getNumarParagraf() { return numarParagraf; }
    QString getLink() { return link; }

    void setNumarCurs(int var) { numarCurs = var; }
    void setNumarPagina(int var) { numarPagina = var; }
    void setNumarParagraf(int var) { numarParagraf = var; }
    void setLink(QString var) { link = var; }

    //polimorfism
    void afisez(int var1){
        qInfo("Suntem la cursul %i", var1);
    }
    void afisez(int var1, int var2){
        qInfo("Suntem la cursul %i, pagina%i\n", var1, var2);
    }
    void afisez(int var1, int var2, int var3){
        qInfo("Cursul %i, pagina: %i, paragraful:%i\n", var1, var2, var3);
    }

    //abstractizare
    virtual void auxiliar(){
        //o folosim mai departe
    }
};

//mostenire
class Final:Destinatie{
private:
    int final;
public:
    int getFinal() { return final; }

    void setFinal(int var) { final = var; }

    void auxiliar() { qInfo() << "Am ajuns la final\n"; }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resetez();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void deschidereDocument(const QString curs)
{
    QDesktopServices::openUrl(QUrl(curs, QUrl::TolerantMode));
}

void fisier();
//variabile globale
Destinatie curs;
int pagini[13], aparitii[13];
int crediteVisibile = 0;

void MainWindow::on_pushButton_1_clicked()
{
    //facem butoanele invizibile
    resetez();

    //facem vizibil labelul unde este rezultatul
    ui->label_3->setVisible(1);

    QFile file("C:/Users/andre/Desktop/ChatBot POO/proiect/resources/test.txt");

    //testam daca primim eroare cand se deschide fisierul in care citim
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    //declarari
    QTextStream in(&file);
    curs.setNumarCurs(0);
    paginiSiAparitii();
    QString raspuns, text, line;
    int gasit = 0;

    //cat timp nu ajunge la final de fisier, citim
    while(!in.atEnd()) {
        line = in.readLine();

        //cautam imputul in sirul curent
        QString searchString(ui->lineEdit->text());

        //pentru ca a citit o linie, marim numarul paragrafului
        curs.setNumarParagraf(curs.getNumarParagraf() + 1);

        //daca gasim CURS inseamna ca trecem la cursul urmator si salvam datele despre cursul anterior
        if(line.contains("CURS", Qt::CaseInsensitive)){
            //incrementam numarul cursurlui si modificam numarul paginii
            curs.setNumarCurs(curs.getNumarCurs() + 1);

            pagini[curs.getNumarCurs()] = curs.getNumarPagina(); //memoram cate pagini are cursul

            curs.setNumarPagina(1);
            //curs.numarParagraf = 0;
        }
        //daca gasim PAGINA inseamna ca am trecut la pagina urmatoare si crestem numarul paginii
        if(line.contains("PAGINA", Qt::CaseInsensitive)){
            curs.setNumarPagina(curs.getNumarPagina() + 1);
            //curs.numarParagraf = 0;
        }

        //verifica daca a gasit in fisier imputul userului
        if (line.contains(searchString, Qt::CaseInsensitive)){
            //transformam numarul cursului in string
            text = QString("%1 ").arg(curs.getNumarCurs());

            //daca nu am gasit pana acum cuvantul cheie in curs, modificam variabila ca sa stim ce afisam
            if(gasit == 0){
                raspuns = "Cuvantul cheie apare in cursurile: ";
                gasit = 1;
            }

            //daca este prima data cand gaseste in acest curs cuvantul cheie, adaugam cursul la raspuns
            if(aparitii[curs.getNumarCurs()] == 0){
                raspuns = raspuns + text;

            }
            //marim numarul de aparitii al cuvantului cheie in curs
            aparitii[curs.getNumarCurs()]++;

            //facem vizibil butonul cursului respectiv pentru a fi deschis
            activezButon(curs.getNumarCurs());

            //memoram numarul paginii unde a fost gasit cuvantul cheie pentru a putea deschide PDF-ul la pagina dorita
            pagini[curs.getNumarCurs()] = curs.getNumarPagina();
        }
    }
    //afisam rezultatul cautarilor
    ui->label_3->setText(raspuns);

    //daca nu a gasit macar o data cuvantul in cursuri afisam mesajul
    if(gasit == 0){
        ui->label_3->setText("Cuvantul cheie nu exista in cursuri");
    }

    //inchidem fisierul de intrare
    file.close();
}

//initializam vectorii paginilor si aparitiilor
void MainWindow::paginiSiAparitii(){
    int i;
    for(i = 1; i <= 12; i++){
        pagini[i] = 1;
        aparitii[i] = 0;
    }
}

//functie pentru a face vizibil butonul cursului in care a fost gasit cuvantul cheie
void MainWindow::activezButon(int numar){
    switch (numar) {
    case 1:
        ui->pushButton_2->setStyleSheet("background-color: green;");
        break;

    case 2:
        ui->pushButton_3->setStyleSheet("background-color: green;");
        break;

    case 3:
        ui->pushButton_4->setStyleSheet("background-color: green;");
        break;

    case 4:
        ui->pushButton_5->setStyleSheet("background-color: green;");
        break;

    case 5:
        ui->pushButton_6->setStyleSheet("background-color: green;");
        break;

    case 6:
        ui->pushButton_7->setStyleSheet("background-color: green;");
        break;

    case 7:
        ui->pushButton_8->setStyleSheet("background-color: green;");
        break;

    case 8:
        ui->pushButton_9->setStyleSheet("background-color: green;");
        break;

    case 9:
        ui->pushButton_10->setStyleSheet("background-color: green;");
        break;

    case 10:
        ui->pushButton_11->setStyleSheet("background-color: green;");
        break;

    case 11:
        ui->pushButton_12->setStyleSheet("background-color: green;");
        break;

    case 12:
        ui->pushButton_13->setStyleSheet("background-color: green;");
        break;
    }
}

//ascundem toate butoanele la inceput de program, deoarece userul nu a cautat ceva inca
void MainWindow:: resetez(){
    ui->pushButton_1->setStyleSheet("background-color: aqua;");
    ui->pushButton_2->setStyleSheet("background-color: red;");
    ui->pushButton_3->setStyleSheet("background-color: red;");
    ui->pushButton_4->setStyleSheet("background-color: red;");
    ui->pushButton_5->setStyleSheet("background-color: red;");
    ui->pushButton_6->setStyleSheet("background-color: red;");
    ui->pushButton_7->setStyleSheet("background-color: red;");
    ui->pushButton_8->setStyleSheet("background-color: red;");
    ui->pushButton_9->setStyleSheet("background-color: red;");
    ui->pushButton_10->setStyleSheet("background-color: red;");
    ui->pushButton_11->setStyleSheet("background-color: red;");
    ui->pushButton_12->setStyleSheet("background-color: red;");
    ui->pushButton_13->setStyleSheet("background-color: red;");

    ui->labelCredits->setVisible(0);
    ui->label_3->setVisible(0);
}


//adaugam eventuri onclick pe butoanele care vor deschide cursurile
void MainWindow::on_pushButton_2_clicked(){
    ui->pushButton_2->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_1.pdf#page=" + QString::number(pagini[1]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_3_clicked(){
    ui->pushButton_3->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_2.pdf#page=" + QString::number(pagini[2]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_4_clicked(){
    ui->pushButton_4->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_3.pdf#page=" + QString::number(pagini[3]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_5_clicked(){
    ui->pushButton_5->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_4.pdf#page=" + QString::number(pagini[4]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_6_clicked(){
    ui->pushButton_6->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_5.pdf#page=" + QString::number(pagini[5]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_7_clicked(){
    ui->pushButton_7->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_6.pdf#page=" + QString::number(pagini[6]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_8_clicked(){
    ui->pushButton_8->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_7.pdf#page=" + QString::number(pagini[7]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_9_clicked(){
    ui->pushButton_9->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_8.pdf#page=" + QString::number(pagini[8]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_10_clicked(){
    ui->pushButton_10->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_9.pdf#page=" + QString::number(pagini[9]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_11_clicked(){
    ui->pushButton_11->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_10.pdf#page=" + QString::number(pagini[9]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_12_clicked(){
    ui->pushButton_12->setEnabled(true);
    QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/curs_11.pdf#page=" + QString::number(pagini[9]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_13_clicked(){
    ui->pushButton_13->setEnabled(true);
    QString text = "file:/// /../proiect/resources/curs_12.pdf#page=" + QString::number(pagini[9]);
    deschidereDocument(text);
}

void MainWindow::on_pushButton_14_clicked()
{
    if(crediteVisibile == 0){
        ui->labelCredits->setVisible(1);
        crediteVisibile = 1;
        QString text = "file:///C:/Users/andre/Desktop/ChatBot POO/proiect/resources/credite.png";
        deschidereDocument(text);
    }
    else{
        ui->labelCredits->setVisible(0);
        crediteVisibile = 0;
    }

}


