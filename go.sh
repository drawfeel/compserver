fuser -v -n tcp 11916 -k;
python manage.py runserver 0.0.0.0:11916
