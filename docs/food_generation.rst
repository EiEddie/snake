===============
Food Generation
===============
---------------
食物生成
---------------

首先划出一个能将蛇身完全包含的最小矩形, 内为 **内部** (*inner*), 外为 **外部** (*outer*).
即如图所示粉红色框:

.. raw:: html

  <style>
  table.fig {
    border-collapse: collapse;
  }

  table.fig td {
    height: 50px;
    width: 50px;
    padding: 0px;
    border: 1px solid #000;
    text-align: center;
  }

  table.fig td[class] {
    background-color: #999;
  }

  table.fig td[box] {
    background-color: #EF9A9A;
  }

  table.fig td[box][class] {
    background-color: #999;
  }

  table.fig td.r {
    border-right: none;
  }

  table.fig td.l {
    border-left: none;
  }

  table.fig td.u {
    border-top: none;
  }

  table.fig td.d {
    border-bottom: none;
  }
  </style>

.. raw:: html

  <table class="fig">
  <tbody>
    <tr>
      <td></td><td></td><td></td><td></td><td></td><td></td><td></td>
    </tr>
    <tr>
      <td></td>
      <td class="d" box></td>
      <td class="r" box>1</td>
      <td class="l" box></td>
      <td box></td><td box></td><td></td>
    </tr>
    <tr>
      <td></td>
      <td class="u d" box></td>
      <td box></td>
      <td class="d" box>2</td>
      <td box></td><td box></td><td></td>
    </tr>
    <tr>
      <td></td>
      <td class="u" box>0</td>
      <td box></td>
      <td class="u" box></td>
      <td class="r" box>3</td>
      <td class="l" box><i>h</i></td>
      <td></td>
    </tr>
    <tr>
      <td></td><td></td><td></td><td></td><td></td><td></td><td></td>
    </tr>
  </tbody>
  </table>

确定此矩形距离原点 (取左上角为原点) 最近与最远的两点 (图中分别为左上与右下角) 的坐标, 就确定了矩形的大小与位置.

容易发现, 所有节的位置 (图中标号处) 与蛇头位置 (图中 `h` 标记处) 横纵坐标的最小值与最大值即对应了上述矩形两角的横纵坐标.

生成食物时, 先尝试在外部随机生成, 如果外部没有空间, 也就是说明内部与整个网格等大时, 在尝试在内部生成.
在内部生成食物时, 先将蛇身所有占用的位置记录下来, 再在未被记录的位置随机生成.
