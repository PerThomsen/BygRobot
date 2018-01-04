<h1>Motorstyring med H-bridge og PWM</h1>
<h3>Hvad er en H-bridge</h3>
En H-bridge er noget elektronik, som ved hjælp af fire relæer (mekaniske eller elektroniske) kan styre en kraftig strøm til en DC-motor ved hjølp af et svagt signal fra fx. en Arduino controller.
Helt enkelt kan den fø motoren til at køre i begge retninger eller stoppe.<br>
H-bridge'en styres med to digitale output pins fra Arduino'en.<br>
Hvis den ene er høj og de anden lav vil motoren køre med fuld kraft.<br>
Hvis høj og lav byttes om, vil motoren køre med fuld kraft i modsat retning.<br>
Hvis begge er høje eller begge er lave vil motoren ikke få strøm.<br>
<h3>Se diagrammet nederst på tegningen.</h3>
De fire cirkler i hjørnerne illustrerer de fire relæer. De to øverste 1H og 3H er en type, som er åbner for gennemstrøm ved højt signal og de to nederste 2L og 4L er åben for gennemstrømnig ved lavt signal.<br>
Hvis 1H og 2L er åbne samtidig vil systyemet kortslutte. Det samme med 3H og 4L. Det kan man undgå ved at de to relæer i samme side aktiveres med med samme signal, så det ene er åbent og det andet lukket.<br>
<h3>Se tabellen øverst på tegningen.</h3>
PWM og 1/0 står for signal i venstre eller højre side af diagrammet nederst. De øvrige kolonner er de fire relæer og motoren.<br>
I anden række har vi altså et eksempel med 1 i venste side og nul i højre side. Det betyder, at relæ 1H og relæ 4L vil være åbne og der vil gå en strøm fra venstre mod højre gennem motoren.<br>
I tredje række er det modsat og motoren vil rotere i modsat retning.<br>
Ifjerde og femte række vil hhv. De to øverste og de to nederste være åbne og der går ikke strøm gennem motoren<br>
<h3>Hvad gør de fire dioder.</h3>
Hvis motoren fortsætter på inertien efter at strømmen er slukket (Fjerde eller femte række i tabellen) vil den virke som dynamo. Uden dioderne vil motorens poler være kortsluttede og den vil bremse kraftigt. Med dioderne kan den løbe ubelastet.<br>
<h2>Hvad er PWM</h2>
PWM står for Pulse With Modulation og er en egenskab for nogle af Arduino's digitale outputs.<br>
<b>Se den lille firkant puls illustration midt på siden med tegninger.</b><br>
Den helt enkle fidus er, at inden for en puls starter signalet højt og skifter til lavt i resten af pulsen og starter så igen højt i næste puls og skifter til lavt osv.<br>
Når pinden aktiveres angiver man et tal mellem 0 og 255, som angiver hvor stor en del af pulsen, som skal være høj. På tegningen er den markeret ved 128, så halvdelen er høj og halvdelen er lav.<br>
Hvis man nu tilslutter højre side af Hbridgen til en PWM udgang med parameter 128 vil den side være høj halvdelen af tiden og lav i den anden halvdel.<br>
Hvis man så tilslutter højre side til en alm.digital udgang, vil motoren køre med halv kraft i den ene eller den anden retning alt efter om denne udgang er høj eller lav.<br>
<b>PWM parameteren på venstre side regulerer hastigheden på motoren.<br>
Høj og lav på højre side angiver retningen.</b>
