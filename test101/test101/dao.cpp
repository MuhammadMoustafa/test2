#include "dao.h"
#include <QtSql/QtSql>

Dao::Dao()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db.db");
    db.open();

    if (db.isOpen() && !db.isOpenError()){
        this->status = "Database opend Seccessfully ... ";
        this->daoIsOpened = true;
    }
    else{
        this->status = db.lastError().text() + " ... ";
        this->daoIsOpened = false;
    }
}

void Dao::dao_createTable(QString& status){

    QSqlQuery query(db);
    QString q = "CREATE TABLE IF NOT EXISTS Patient("  \
                    "ID             INTEGER PRIMARY KEY," \
                    "Name           TEXT    NOT NULL," \
                    "Age            INT     NOT NULL," \
                    "Address        TEXT);";
    query.prepare(q);
    if (!query.exec()){
        status += query.lastError().text();
    }
    else{
       status += "Table created successfully ...";
    }

    q = "CREATE TABLE IF NOT EXISTS Image_Table ("
        "Patient_ID INTEGER,"
        "Filename TEXT, "
        "Image_Data BLOB,"
        "FOREIGN KEY (Patient_ID) REFERENCES Patient(ID),"
        "PRIMARY KEY (Patient_ID, Filename) )";

    query.prepare(q);

    if (!query.exec()){
       status += query.lastError().text();
    }
    else{
       status += "Table created successfully";
    }

}

void Dao::dao_insert(Person person, QString& status){

    QSqlQuery query(db);
    query.prepare("INSERT INTO Patient (ID, Name, Age, Address) "\
                   "VALUES (NULL, :name, :age, :address)");

    query.bindValue(":name", person.name);
    query.bindValue(":age", person.age);
    query.bindValue(":address", person.address);

    if(! query.exec()){
        status = query.lastError().text();
    }
    else{
        status = "Record Inserted Successfully";
    }
}

QSqlQuery Dao::dao_select(QString& status){

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Patient");
    /**
    Not a good practise .... Select * return all fileds and records whithout a specific order
    use -> SELECT column_name1, column_name2, column_name3 FROM table_name <- instead

    writing all reserved words in sql as UPPERCASE is a good practise although sql is case insensitive.
    */
    if (!query.exec()){
        status = query.lastError().text();
    }
    else{
        if(query.first()){
            status = "Query Executed Successfully";
        }

        else{
            status = "Database is empty";
        }
    }
    return query;
}

QSqlQuery Dao::dao_select(int id, QString& status){

    QSqlQuery query;

    query.prepare("SELECT * FROM Patient where ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()){
        status = query.lastError().text();
    }

    else{
        if (query.first()){
            status = "Query Executed Successfully";
            //qDebug() << query.record.count();
        }

        else{
            status = "No Such ID";
        }
    }
    return query;
}
QSqlQuery Dao::dao_select(QString name, QString& status){

    QSqlQuery query;
    query.prepare("SELECT * FROM Patient where Name like :name");
    query.bindValue(":name", '%' + name + '%');

    if (!query.exec()){
        status = query.lastError().text();
    }

    else{
        if (query.first()){
            status = "Query Executed Successfully";
        }

        else{
            status = "No Such Name";
        }
    }
    return query;
}


void Dao::dao_delete(int id, QString& status, bool& queryIsExecuted){

    QSqlQuery query;
    query.prepare("DELETE FROM Patient WHERE ID = :id");
    query.bindValue(":id", id);
    if (!query.exec()){
        status = query.lastError().text();
        queryIsExecuted = false;
    }

    else if (query.numRowsAffected() == 0){
        status = "NO Such ID";
        queryIsExecuted = false;
    }

    else{
        status = "Record Deleted Successfully";
        queryIsExecuted = true;

        query.prepare("DELETE FROM Image_Table WHERE Patient_ID = :id");
        query.bindValue(":id", id);

        if (!query.exec()){
            status += query.lastError().text();
        }
        else{
            status += "and images";
        }
    }

}

/**
  http://www.sqlite.org/cvstrac/wiki?p=BlobExample
 */

void Dao::dao_insert_image(Patient_Image image, QString& status){
    QSqlQuery query;

    query.prepare( "INSERT INTO Image_Table (Patient_ID, Filename, Image_Data) VALUES (:id, :filename, :imageData)" );

    query.bindValue( ":id", image.id );
    query.bindValue( ":filename", image.filename );
    query.bindValue( ":imageData", image.image );

    if( !query.exec() ){
        status = "Error inserting image into table:" + query.lastError().text();
    }

    else{
        status = "Image inserted successfully";
    }

}

QVector<QByteArray> Dao::dao_select_image(int id, QString &status){

    QSqlQuery query;
    QVector<QByteArray> outByteArray;
    query.prepare("SELECT Image_Data from Image_Table WHERE Patient_ID = :id");
    query.bindValue(":id", id);

    if( !query.exec()){
        status = "Error getting image from table" + query.lastError().text();
    }
    else{
        if(query.first()){
            status = "Images selected successfully";
            do{
                outByteArray.append( query.value(0).toByteArray() );
            }while(query.next());
        }
        else{
            status = "Database is empty";
        }
    }
    return  outByteArray;
}

Dao::~Dao(){
    db.close();
}
