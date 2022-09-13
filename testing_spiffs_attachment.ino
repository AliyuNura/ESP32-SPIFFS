#include "SPIFFS.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "aliyu"
#define WIFI_PASSWORD "******##"

#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465

#define sender_email "aliyunuraesp32@gmail.com"
#define sender_password "xejycdieyfaxbikp"

#define Recipient_email "engrsnaliyu@gmail.com"
SMTPSession smtp;

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
    Serial.println("SPIFFS mounted successfully");
  }

  smtp.debug(1);

  ESP_Mail_Session session;

  session.server.host_name = SMTP_server ;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "mydomain.net";

  SMTP_Message message;

  message.enable.chunking = true;


  message.sender.name = " Aliyu's ESP32 Mail";
  message.sender.email = sender_email;

  message.subject = "ESP32 Testing Email with Attachments";
  message.addRecipient("aliyu",Recipient_email);


  String htmlMsg = "This attachment message was sent by ESP32 board";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  SMTP_Attachment attachment;

  attachment.descr.filename = "image.png";
  message.resetAttachItem(attachment);
  attachment.descr.mime = "iot/png"; 
  attachment.file.path = "/iot.png";
  attachment.file.storage_type = esp_mail_file_storage_type_flash;
  attachment.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  message.addAttachment(attachment);

  attachment.descr.filename = "test.txt";
  attachment.descr.mime = "test/plain";
  attachment.file.path = "/test.txt";
  attachment.file.storage_type = esp_mail_file_storage_type_flash;
  attachment.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  message.addAttachment(attachment);

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message, true))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void loop()
{
}
