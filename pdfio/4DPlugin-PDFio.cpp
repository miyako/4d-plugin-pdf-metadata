/* --------------------------------------------------------------------------------
 #
 #  4DPlugin-PDFio.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : PDFio
 #	author : miyako
 #	2023/05/15
 #  
 # --------------------------------------------------------------------------------*/

#include "4DPlugin-PDFio.h"

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params) {
    
	try
	{
        switch(selector)
        {
			// --- PDFio
            
			case 1 :
				PDFio_Get_metadata(params);
				break;

        }

	}
	catch(...)
	{

	}
}

#pragma mark -

static bool error_cb(pdfio_file_t *pdf, const char *message, void *data)
{
  return (false);
}

static const char *password_cb(void *data, const char *filename)
{
    if(data != NULL) {
        return (const char *)data;
    }

    return "";
}

static bool get_media_box(pdfio_file_t *pdf,
                          pdfio_rect_t *media_box,
                          pdfio_rect_t *crop_box)
{
    
    memset(media_box, 0, sizeof(pdfio_rect_t));
    memset(crop_box, 0, sizeof(pdfio_rect_t));
    
    if(pdf != NULL) {
        if(pdfioFileGetNumPages(pdf) != 0) {
            pdfio_obj_t *page = pdfioFileGetPage(pdf, 0);
            bool ph1 = false;
            pdfio_dict_t *dict = pdfioObjGetDict(page);
            if (!pdfioDictGetRect(dict, "MediaBox", media_box))
            {
                if ((page = pdfioDictGetObj(dict, "Parent")) != NULL)
                {
                    dict = pdfioObjGetDict(page);
                    if(pdfioDictGetRect(dict, "MediaBox", media_box))
                    {
                        ph1 =true;
                    }
                }
            }else{
                ph1 =true;
            }
            if(ph1)
            {
                dict = pdfioObjGetDict(page);
                if (!pdfioDictGetRect(dict, "CropBox", crop_box))
                {
                    if ((page = pdfioDictGetObj(dict, "Parent")) != NULL)
                    {
                        dict = pdfioObjGetDict(page);
                        if(pdfioDictGetRect(dict, "CropBox", crop_box))
                        {
                            return true;
                        }
                    }
                }else{
                    return true;
                }
            }
        }
    }
    return false;
}

static void PDFio_SET_METADATA(PA_PluginParameters params) {

    PackagePtr pParams = (PackagePtr)params->fParameters;
    
    C_TEXT Param1;
    C_TEXT Param3;
    
    Param1.fromParamAtIndex(pParams, 1);
    Param3.fromParamAtIndex(pParams, 3);
    
    CUTF8String path;
    Param1.copyPath(&path);
    
    PA_ObjectRef meta = PA_GetObjectParameter(params, 2);
    
    if(meta != NULL) {
        CUTF8String password;
        Param3.copyPath(&password);
        
        pdfio_file_t *pdf = pdfioFileOpen((const char *)path.c_str(),
                                          password_cb,
                                          (void *)password.c_str(),
                                          NULL,
                                          (void *)"");
        
        
        if(pdf != NULL){
            
            const char *clone = "copy.pdf";
            
            pdfio_rect_t media_box;
            pdfio_rect_t  crop_box;
            if(get_media_box(pdf, &media_box, &crop_box)) {
                pdfio_file_t *pdf2 = pdfioFileCreate(clone,
                                                     pdfioFileGetVersion(pdf),
                                                     &media_box,
                                                     &crop_box,
                                                     NULL,
                                                     NULL);
                if(pdf2 != NULL) {

                    size_t i;
                    size_t count;
                    for (i = 0, count = pdfioFileGetNumPages(pdf); i < count; ++i)
                    {
                        pdfio_obj_t *page = pdfioFileGetPage(pdf, i);
                        pdfioPageCopy(pdf2, page);
                    }
                    for (i = 0, count = pdfioFileGetNumObjs(pdf); i < count; ++i)
                    {
                        pdfio_obj_t *obj = pdfioFileFindObj(pdf, i);
                        pdfio_obj_t *obj2 = pdfioObjCopy(pdf2, obj);
                        if(obj2 != NULL) {
                            pdfioObjClose(obj2);
                        }
                    }
                    

                    CUTF8String author;
                    if(ob_get_s(meta, L"author", &author)) {
                        pdfioFileSetAuthor(pdf2, (const char *)author.c_str());
                    }
                    CUTF8String creator;
                    if(ob_get_s(meta, L"creator", &creator)) {
                        pdfioFileSetCreator(pdf2, (const char *)creator.c_str());
                    }
                    //pdfioFileSetCreationDate
                    CUTF8String keywords;
                    if(ob_get_s(meta, L"keywords", &keywords)) {
                        pdfioFileSetKeywords(pdf2, (const char *)keywords.c_str());
                    }
                    //pdfioFileSetPermissions
                    CUTF8String subject;
                    if(ob_get_s(meta, L"subject", &subject)) {
                        pdfioFileSetSubject(pdf2, (const char *)subject.c_str());
                    }
                    CUTF8String title;
                    if(ob_get_s(meta, L"title", &title)) {
                        pdfioFileSetTitle(pdf2, (const char *)title.c_str());
                    }
                    
                    pdfioFileClose(pdf2);
                }
            }

            pdfioFileClose(pdf);
        }
    }
}

static void time_to_utc(time_t lt, std::string& timeString) {
    
    auto field = *std::gmtime(&lt);
    field.tm_isdst = -1;
    std::vector<char>buf(30);
    std::strftime(&buf[0], buf.size(), "%FT%TZ", &field);
    timeString = &buf[0];
}

static void PDFio_Get_metadata(PA_PluginParameters params) {

    PackagePtr pParams = (PackagePtr)params->fParameters;
    
    PA_ObjectRef status = PA_CreateObject();
    ob_set_b(status, L"success", false);
    
    C_TEXT Param1;
    C_TEXT Param2;
    
    Param1.fromParamAtIndex(pParams, 1);
    Param2.fromParamAtIndex(pParams, 2);
    
    CUTF8String path;
    Param1.copyPath(&path);
    
    CUTF8String password;
    Param2.copyPath(&password);
    
    pdfio_file_t *pdf = pdfioFileOpen((const char *)path.c_str(),
                                      password_cb,
                                      (void *)password.c_str(),
                                      NULL,
                                      (void *)"");
    
    if(pdf != NULL){
        
        ob_set_b(status, L"success", true);
        
        PA_ObjectRef meta = PA_CreateObject();
        
        ob_set_s(meta, L"version", pdfioFileGetVersion(pdf));
        ob_set_s(meta, L"title", pdfioFileGetTitle(pdf));
        ob_set_s(meta, L"subject", pdfioFileGetSubject(pdf));
        ob_set_s(meta, L"producer", pdfioFileGetProducer(pdf));
        
        pdfio_encryption_t encryption;
        pdfio_permission_t permission = pdfioFileGetPermissions(pdf, &encryption);
        
        PA_ObjectRef perm = PA_CreateObject();
        
        ob_set_b(perm, L"print", permission & PDFIO_PERMISSION_PRINT);
        ob_set_b(perm, L"modify", permission & PDFIO_PERMISSION_MODIFY);
        ob_set_b(perm, L"copy", permission & PDFIO_PERMISSION_COPY);
        ob_set_b(perm, L"annotate", permission & PDFIO_PERMISSION_ANNOTATE);
        ob_set_b(perm, L"forms", permission & PDFIO_PERMISSION_FORMS);
        ob_set_b(perm, L"reading", permission & PDFIO_PERMISSION_READING);
        ob_set_b(perm, L"assemble", permission & PDFIO_PERMISSION_ASSEMBLE);
        ob_set_b(perm, L"printHigh", permission & PDFIO_PERMISSION_PRINT_HIGH);
        ob_set_o(meta, L"permission", perm);
        
        switch (encryption) {
            case PDFIO_ENCRYPTION_NONE:
                ob_set_s(meta, L"encryption", "NONE");
                break;
            case PDFIO_ENCRYPTION_AES_128:
                ob_set_s(meta, L"encryption", "AES_128");
                break;
            case PDFIO_ENCRYPTION_AES_256:
                ob_set_s(meta, L"encryption", "AES_256");
                break;
            case PDFIO_ENCRYPTION_RC4_128:
                ob_set_s(meta, L"encryption", "RC4_128");
                break;
            case PDFIO_ENCRYPTION_RC4_40:
                ob_set_s(meta, L"encryption", "RC4_40");
                break;
            default:
                break;
        }
        
        ob_set_n(meta, L"pageCount", pdfioFileGetNumPages(pdf));
        ob_set_s(meta, L"name", pdfioFileGetName(pdf));
        ob_set_s(meta, L"keywords", pdfioFileGetKeywords(pdf));
        ob_set_s(meta, L"creator", pdfioFileGetCreator(pdf));
        std::string creationDate;
        time_to_utc(pdfioFileGetCreationDate(pdf), creationDate);
        ob_set_s(meta, L"creationDate", creationDate.c_str());
        ob_set_s(meta, L"author", pdfioFileGetAuthor(pdf));
        
        ob_set_o(status, L"meta", meta);
        pdfioFileClose(pdf);
    }

    PA_ReturnObject(params, status);
}

