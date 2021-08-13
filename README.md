# LXL Public Upgrade Service

### Intention
此服务意为节省开发者寻找相关托管服务器的时间或金钱，为插件开发提供更多方便。

### Join
 - 不需要批准，自行新建一个属于你的插件的文件夹，每个人可以拥有多个文件夹，每个插件只能有一个文件夹。
 - 以后，您将负责维护此文件夹，可以在不违反Github存储库相关规定的前提下自由增减文件夹内内容。
 - 不要去修改别人的文件夹，除非得到授意，如您发现有人擅自修改您的文件夹内容，请向我们提出举报

### Usage
 - 例如您在**Test**文件夹新建了**version.json**文件。稍后，您可以使用以下几个链接访问到您的文件。
 - 不一定是Json文件，您可以随意修改文件夹内容，插件的源文件也可以上传。

链接 | 说明
-|-
https://lxl-upgrade.amd.rocks/Test/version.json | 使用自选Cloudflare节点，动态缓存控制
https://cdn.jsdelivr.net/gh/LiteLDev-LXL/Update/Test/version.json | 使用Jsdelivr，有大陆节点，可能存在缓存

### Caching mechanism
 - 如果您使用AMD域名，则可以使用此缓存控制机制。
 - 缓存将明显提升下载速度，在非必要情况下不要使用**nocache**。而Json文件体积较小，为了实时生效，可以不进行缓存。

实时生效 | 路径 | 说明
-|-|-
无缓存 | /Test/*.json | 所有在文件夹下一级目录的Json文件不会缓存
无缓存 | /Test/nocache/* | 在文件夹的nocache目录下的所有文件不会缓存

### Auto MD5 Calculation

- 此仓库拥有自动MD5计算 actions，在每次push时触发，扫描所有更改文件并重新计算其MD5值上传到仓库，方便自动更新时作下载校验之用
- MD5将被储存到文件名.md5的文本文件中，储存于原文件的同一目录中，下载时带上此文件一起下载并校验即可
- 默认情况下，MD5计算会跳过所有的json文件
- 如果你有目录需要被排除，请按同样的格式将目录路径写入到根目录IgnoreDirs.conf即可，一行一个目录路径，并push到仓库中
