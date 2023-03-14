
from django.db import models
from urllib import request
import datetime
import os
from django.urls import reverse

# Create your models here.
def getFileName(request, filename, UploadFolder):

    origiFilename = filename
    timeNow = datetime.datetime.now().strftime('%Y%m%d%H:%M:%S')
    timeNow_dir = datetime.datetime.now().strftime('%Y%m_%d')
    filename = "%s%s" % (timeNow, origiFilename)
    upload_to = 'upload/' + UploadFolder + '/'
    return os.path.join(upload_to, timeNow_dir)


class HearoImage(models.Model):

    image = models.ImageField(null=True, blank=True, upload_to=getFileName(request=request, filename=id, UploadFolder="hearoImage"))
    updated = models.DateTimeField(auto_now=True, auto_now_add=False)
    created = models.DateTimeField(auto_now_add=True, auto_now=False)

    class Meta:
       pass
    def __str__(self):
        return str(self.id)
    @property
    def imageURL(self):
        try:
            url = self.image.url
        except:
            url = ''
        return url
    # def get_absolute_url(self):
    #     return reverse("_detail", kwargs={"pk": self.pk})
    def delete(self):
        return reverse(
            "heroimage:delete-image-herooo",
            kwargs = {
                "imageId":self.pk
            }
        )