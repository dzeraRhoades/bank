# bank
Программа имитирует один рабочий день банка и состоит из нескольких важных этапов:
1) Инициализация. Создаются все необходимые объекты, выделяется память.
2) Работа программы. Осново=ной метод включает в себя цикл, каждая итерация которого представляет из себя одну минуту работы банка. На каждой итерации происходят несколько важных событий: во первых, приходят (с 8 до 12) и уходят (с 5 до 9) работники банка, затем поступает случайно сгенерированное число заявок (каждая в определенный отдел), затем заявки распределяются между работниками каждого отделения. Если количество заявок в отделении слишком велико, то выполняется поиск самого свободного отделения и все заявки переходят туда. После того, как сотрудник выполнил заявку, он может уйти на обед (только в обеденные часы и только один раз). Каждая заявка обладает уровнем важности, в соответсвии с чем сначала обрабатываются более важные заявки. Поэтому для хранения необработанных заявок в приложении используется биномиальная куча (собственной реализации, в проекте также присутствуют и другие виды куч, но используется только одна).
Информация о приеме и выолнении заявок, а также о всех действиях сотрудников логируется в log файлы в папке logs.
3) Все выделенные ресурсы очищаются приложение завершает работу.
