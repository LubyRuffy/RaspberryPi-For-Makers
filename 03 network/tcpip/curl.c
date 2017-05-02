#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>

int main()
{
	CURLcode rc;
	FILE *fh;
	const char* url = "http://www.kma.go.kr/weather/observation/currentweather.jsp";
	curl_global_init( CURL_GLOBAL_ALL ) ;
	CURL* ctx = curl_easy_init() ;
	if( NULL == ctx ){
		printf("cURL �ʱ�ȭ ����\n");
		return -1;
	}
	curl_easy_setopt(ctx, CURLOPT_URL, url);
	
	//�������� ��� ������ ���Ϸ� �����Ѵ�. ���� ���� ��� ȭ�鿡 ����Ϸ��� 
	//curl_easy_setopt�Լ����� fh ��� stdout�� ����ϸ� �ȴ�.

	fh = fopen("web_croll.txt", "a+");
	curl_easy_setopt( ctx , CURLOPT_WRITEDATA , fh) ;
	rc = curl_easy_perform(ctx);
	if (CURLE_OK != rc)
	{
		printf("cURL URL ȣ�� ����\n");
	}
	else
	{
		// get some info about the xfer:
		double filesize = 0 ;
		long response ;
		char* str = NULL ;

		if( CURLE_OK == curl_easy_getinfo( ctx , CURLINFO_HTTP_CODE , &response ) ){
			printf( "\nResponse code: %d\n " ,response);
		}

		if( CURLE_OK == curl_easy_getinfo( ctx , CURLINFO_CONTENT_TYPE , &str ) ){
			printf( "Content type: %s\n " ,str);
		}

		if( CURLE_OK == curl_easy_getinfo( ctx , CURLINFO_SIZE_DOWNLOAD , &filesize ) ){
			printf( "Download size: %d bytes\n " ,filesize);
		}
	}
	curl_easy_cleanup( ctx ) ;
	curl_global_cleanup() ;
	fclose(fh);
	return 0;
}