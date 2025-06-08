FROM nginx:latest

# Копируем конфигурационные файлы из хоста в контейнер
COPY default.conf /etc/nginx/conf.d/default.conf
COPY nginx.conf /etc/nginx/nginx.conf

# Открываем порты
EXPOSE 80

# Запускаем Nginx
CMD ["nginx", "-g", "daemon off;"]