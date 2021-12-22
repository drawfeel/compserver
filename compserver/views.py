
from django.http import HttpResponse
from django.shortcuts import render

def hello(request):
    print ("hello")
    return render(request, 'hello.html')
