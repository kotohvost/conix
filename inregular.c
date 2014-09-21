/****************************************************************************/
			inregular(str, expr)
/*              Проверка регулярных выражений
*****************************************************************************/
	char    *str;   /* проверяемая строка */
	char    *expr;  /* регулярное выражение */
{
	char    *ptr=str;
	char    *reg=expr;
/*==========================================================================*/

if(!*str){
  while(*expr=='*') expr++;
  if(*expr) return(0); else return(1);
}

if(*expr=='*')
  {
    while(*str)  if(inregular(str++, expr+1)) return(1);
    return(inregular(str, expr+1));
  }else{
    while(*str==*expr && *str && *expr && *expr!='*') { str++; expr++;}
    if(*expr=='*')
      {
	return(inregular(str, expr));
      }else{
	return(*str=='\0' && *expr=='\0');
      }
  }
}
