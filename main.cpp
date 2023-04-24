#include "mongoose.h"
#include <iostream>
#include <regex>


#include <mysqlx/xdevapi.h>

#include "Student.h"

std::vector<Student> students;

static void HandlePageNotFound(mg_connection *nc,   mg_http_message *hm)
{
	mg_printf(nc, "HTTP/1.1 404 Not Found\r\n"
				  "Content-Type: text/plain\r\n"
				  "\r\n"
				  "The requested resource could not be found.");
}

static void HandleHomePage(mg_connection *nc,   mg_http_message *hm)
{
	mg_printf(nc, "HTTP/1.1 200 OK\r\n"
				  "Content-Type: text/html\r\n"
				  "\r\n"
				  "<html><body>"
				  "<h1>Welcome to the home page!</h1>"
				  "</body></html>\n");
}

static void HandleHelloName( mg_connection *nc,   mg_http_message *hm,std::string name)
  {
	  mg_printf(nc, "HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"\r\n"
					"<html><body>"
					"<h1>Hello %s</h1>"
					"</body></html>\n",name.c_str());
}

static void HandleStudents(struct mg_connection *nc,   mg_http_message *hm)
{
	mg_printf(nc, "HTTP/1.1 200 OK\r\n""Content-Type: text/html\r\n""\r\n""<html><body>");

	for (const auto& student:students)
	{
		mg_printf(nc,"%d   %s	%s	%s	%s <br>",student.ID,student.first_name.c_str(),student.middle_name.c_str(),student.second_name.c_str(),student.email.c_str());
	}
	mg_printf(nc,"</body></html>\n");
}

static void HandleStudentID(struct mg_connection *nc,   mg_http_message *hm,uint32_t id)
{
	id-=1;
	mg_printf(nc, "HTTP/1.1 200 OK\r\n""Content-Type: text/html\r\n""\r\n""<html><body>");
	mg_printf(nc,"%d   %s	%s	%s	%s ",students[id].ID,students[id].first_name.c_str(),students[id].middle_name.c_str(),students[id].second_name.c_str(),students[id].email.c_str());
	mg_printf(nc,"</body></html>\n");
}

static void HandelRequest(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
  {

	if (ev == MG_EV_HTTP_MSG)
	{
		mg_http_message * hm = (mg_http_message *)ev_data;
		std::cmatch m;
		std::regex pattern_hello("/hello/.*");
		std::regex pattern_student("/students/[1-9]");

		std::string url = std::string(hm->uri.ptr,hm->uri.len);

		if(mg_http_match_uri(hm,"/home"))
		{
			HandleHomePage(c,hm);
			std::cout<<"home"<<std::endl;
		}
		else if(std::regex_match(url.c_str(),m,pattern_hello))
		{
			std::string name = url.substr(7);
			HandleHelloName(c,hm,name);
			std::cout<<"hello"<<std::endl;
		}
		else if(mg_http_match_uri(hm,"/students"))
		{
			HandleStudents(c,hm);
			std::cout<<"students"<<std::endl;
		}
		else if(std::regex_match(url.c_str(),m,pattern_student))
		{
			std::string ID_s = url.substr(10);
			uint32_t ID = (uint32_t)std::stoi(ID_s);
			if(students.size()>=ID)
			{
				HandleStudentID(c,hm,ID);
				std::cout<<"Student ID"<<std::endl;
			}
			else
			{
				std::cout<<"Student ID too big"<<std::endl;
			}
		}
		else
		{
			HandlePageNotFound(c,hm);
			std::cout<<"Not found"<<std::endl;
		}
	}
}

int main(int argc, char *argv[])
{
	try
	{
		mysqlx::SessionSettings ss("db",33060,"root","secret", "kb39");
		mysqlx::Session session(ss);

		mysqlx::Table myTable = session.getSchema("kb39").getTable("students");
		mysqlx::RowResult  res = myTable.select("*").execute();


			for(const auto& row : res)
			{

				std::cout << "id = " << row[0]<<std::endl;
				std::cout << "Name " << row[1]<<std::endl;
				std::cout << "MiddleName " << row[2]<<std::endl;
				std::cout << "LastName " << row[3]<<std::endl;
				std::cout << "Email " << row[4]<<"\n"<<std::endl;

				Student student;
				student.ID=row[0];
				student.first_name=(std::string)row[1];
				student.middle_name=(std::string)row[2];
				student.second_name=(std::string)row[3];
				student.email=(std::string)row[4];


				students.push_back(student);
			}
	}
	catch (mysqlx::Error &e)
	{
		std::cout << " MySQL error code: " << e.what()<<std::endl;
		return EXIT_FAILURE;
	}
	catch(...)
	{
		return EXIT_FAILURE;
	}

	std::cout<<"Set up"<<std::endl;
	mg_mgr mgr;
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://0.0.0.0:8000", HandelRequest, &mgr);

	for (;;)
	{
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);


	return 0;
}



