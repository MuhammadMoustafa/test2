#include "Headers/image.h"

image::image(){

}

QString image::get_comment(){
    return this->comment;
}


QString image::get_filename(){
    return this->filename;
}

QByteArray image::get_image_data(){
    return this->image_data;
}

int image::get_image_id(){
    return this->image_id;}

int image::get_visit_id(){
    return this->visit_id;}

QString image::get_patient_id(){
    return this->patient_id;
}

QChar image::get_tsukuba_score(){
    return this->tsukuba_score;
}


void image::set_comment(QString comment){
    this->comment = comment;
}

void image::set_visit_id(int visit_id){
   this->visit_id = visit_id;
}

void image::set_filename(QString filename){
    this->filename = filename;
}

void image::set_image_data(QByteArray image_data){
    this->image_data = image_data;
}

void image::set_image_id(int image_id){
    this->image_id = image_id;
}

void image::set_patient_id(QString patient_id){
    this->patient_id = patient_id;
}

void image::set_tsukuba_score(QChar tsukuba_score){
    this->tsukuba_score = tsukuba_score;
}
