#include "mongoose.h"
#include <iostream>
#include <regex>

//#include <mysql/jdbc.h>

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
	/*try
	{

	sql::mysql::MySQL_Driver* driver;
	sql::Connection * con;

	driver = sql::mysql::get_mysql_driver_instance();

		sql::ConnectOptionsMap connection_properties;
		connection_properties["hostName"] = "tcp://localhost:3306";
		connection_properties["userName"] = "Oleh";
		connection_properties["password"] = "A-z0976606041";
		connection_properties["schema"] = "kb39";
		connection_properties["port"] = 3306;
		connection_properties["OPT_RECONNECT"] = true;

	con = driver->connect(connection_properties);


	if(con->isValid()==0)
	{
		std::cout<<"Failed to connect"<<std::endl;
	}


	sql::Statement *stmt;
	sql::ResultSet  *res;

	stmt = con->createStatement();
	stmt->execute("USE kb39");
	res=stmt->executeQuery("SELECT * FROM students");

	while(res->next())
	{
		std::cout << "id = " << res->getInt(1)<<std::endl;
		std::cout << "Name " << res->getString("FirstName")<<std::endl;
		std::cout << "MiddleName " << res->getString("MiddleName")<<std::endl;
		std::cout << "LastName " << res->getString("SecondName")<<std::endl;
		std::cout << "Email " << res->getString("Email")<<"\n"<<std::endl;

		Student student;
		student.ID=res->getInt(1);
		student.first_name=res->getString("FirstName");
		student.middle_name=res->getString("MiddleName");
		student.second_name=res->getString("SecondName");
		student.email=res->getString("Email");

		students.push_back(student);
	}
}
catch (sql::SQLException &e)
{
	std::cout << "# ERR: SQLException in " << __FILE__<<std::endl;
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

	return EXIT_FAILURE;
}*/

	std::cout<<"Set up"<<std::endl;
	mg_mgr mgr;
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://127.0.0.1:8000", HandelRequest, &mgr);

	for (;;)
	{
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);


	return 0;
}



