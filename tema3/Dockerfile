FROM python:3.10
COPY requirements.txt /tmp
RUN pip install -U setuptools
RUN pip install -r /tmp/requirements.txt
COPY /src /app
WORKDIR /app
CMD ["python", "message_adapter.py"]