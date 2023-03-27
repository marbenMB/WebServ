package main

import (
    "fmt"
    "os"
    "strings"
)

// -- query function --------------//
func queryVar(queryString string) map[string]string {
    if queryString != "" {
        queryParams := strings.Split(queryString, "&")
        paramsDict := make(map[string]string)
        for _, param := range queryParams {
            parts := strings.Split(param, "=")
            paramsDict[parts[0]] = parts[1]
        }
        return paramsDict
    }
    return nil
}
// * -- end query function  --------//


//  -- template function ----------//
func getPage(paramsDict map[string]string) string {
    var html string
    var title string
    var heading string
    var message string
    if paramsDict != nil {
        title = paramsDict["title"]
        heading = paramsDict["heading"]
        message = paramsDict["message"]
    }
    if os.Getenv("VALID_COOKIE") == "NO" {
        template, err := os.ReadFile("./public/assets/no_cookie.html")
        if err != nil {
            fmt.Println(err)
            return ""
        }
        html = string(template)
    } else if len(paramsDict) >= 3 {
        template, err := os.ReadFile("public/assets/template.html")
        if err != nil {
            fmt.Println(err)
            return ""
        }
        html = string(template)
        html = strings.ReplaceAll(html, "{title}", title)
        html = strings.ReplaceAll(html, "{heading}", heading)
        html = strings.ReplaceAll(html, "{message}", message)
    } else {
        template, err := os.ReadFile("public/assets/post_template.html")
        if err != nil {
            fmt.Println(err)
            return ""
        }
        html = string(template)
    }
    return html
}

// * -- end template function ------//



func main() {
    contentType := os.Getenv("CONTENT_TYPE")
    if contentType == "" {
            contentType = "text/plain"
        }
    queryString := os.Getenv("QUERY_STRING")
    bodyString := os.Getenv("REQUEST_BODY")
    requestMethod := os.Getenv("REQUEST_METHOD")
    queryData := queryVar(queryString)
    bodyData := queryVar(bodyString)
    
    var html string
    if requestMethod == "GET" {
        html = getPage(queryData)
    } else {
        html = getPage(bodyData)
    }
    fmt.Println(html)
    }
